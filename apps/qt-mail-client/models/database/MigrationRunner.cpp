#include "headers/database/MigrationRunner.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

#include "headers/database/Statement.h"

namespace Storage {

MigrationRunner::MigrationRunner(Database& database, const std::filesystem::path& migrations_path) :
  m_database(database),
  m_migrations_path(migrations_path)
{
}

void MigrationRunner::Run()
{
  EnsureMigrationsTable();

  for (const Migration& migration : LoadMigrations())
  {
    ApplyMigration(migration);
  }
}

void MigrationRunner::EnsureMigrationsTable()
{
  m_database.Execute(
    R"SQL(
			CREATE TABLE IF NOT EXISTS schema_migrations (
				version INTEGER PRIMARY KEY,
				name TEXT NOT NULL,
				applied_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP
			);
		)SQL");
}

std::vector<MigrationRunner::Migration> MigrationRunner::LoadMigrations() const
{
  std::error_code error_code;

  if (!std::filesystem::is_directory(m_migrations_path, error_code))
  {
    throw std::runtime_error("Failed to read migrations directory: " + m_migrations_path.string());
  }

  std::vector<Migration> migrations;
  std::filesystem::directory_iterator iterator(m_migrations_path, error_code);

  for (; iterator != std::filesystem::directory_iterator(); iterator.increment(error_code))
  {
    if (error_code)
    {
      throw std::runtime_error("Failed to read migrations directory: " +
                               m_migrations_path.string());
    }

    const std::filesystem::directory_entry& entry = *iterator;

    if (!entry.is_regular_file() || entry.path().extension() != ".sql")
    {
      continue;
    }

    const std::string filename = entry.path().filename().string();
    migrations.push_back({ParseVersion(filename), filename, entry.path()});
  }

  if (error_code)
  {
    throw std::runtime_error("Failed to read migrations directory: " + m_migrations_path.string());
  }

  std::sort(migrations.begin(),
            migrations.end(),
            [](const Migration& left, const Migration& right)
            {
              if (left.version != right.version)
              {
                return left.version < right.version;
              }

              return left.name < right.name;
            });

  for (std::size_t index = 1; index < migrations.size(); ++index)
  {
    if (migrations[index - 1].version == migrations[index].version)
    {
      throw std::runtime_error("Duplicate migration version: " +
                               std::to_string(migrations[index].version));
    }
  }

  return migrations;
}

void MigrationRunner::ApplyMigration(const Migration& migration)
{
  const std::string sql = ReadMigration(migration.path);
  m_database.Execute("BEGIN IMMEDIATE;");

  try
  {
    if (!IsMigrationApplied(migration.version))
    {
      m_database.Execute(sql);
      RecordMigration(migration);
    }

    m_database.Execute("COMMIT;");
  }
  catch (...)
  {
    try
    {
      m_database.Execute("ROLLBACK;");
    }
    catch (...)
    {
    }

    throw;
  }
}

bool MigrationRunner::IsMigrationApplied(int version) const
{
  Statement statement(m_database,
                      R"SQL(
			SELECT 1
			FROM schema_migrations
			WHERE version = ?
			LIMIT 1;
		)SQL");
  statement.BindInt(1, version);

  return statement.Step();
}

void MigrationRunner::RecordMigration(const Migration& migration)
{
  Statement statement(m_database,
                      R"SQL(
			INSERT INTO schema_migrations (
				version,
				name
			)
			VALUES (?, ?);
		)SQL");
  statement.BindInt(1, migration.version);
  statement.BindText(2, migration.name);
  statement.Step();
}

std::string MigrationRunner::ReadMigration(const std::filesystem::path& path) const
{
  std::ifstream file(path);

  if (!file.is_open())
  {
    throw std::runtime_error("Failed to read migration file: " + path.string());
  }

  const std::string sql{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

  if (file.bad())
  {
    throw std::runtime_error("Failed to read migration file: " + path.string());
  }

  return sql;
}

int MigrationRunner::ParseVersion(const std::string& filename) const
{
  const std::size_t separator_position = filename.find('_');

  if (separator_position == std::string::npos || separator_position == 0 ||
      filename.size() <= separator_position + 5 || filename.substr(filename.size() - 4) != ".sql")
  {
    throw std::runtime_error("Invalid migration filename: " + filename);
  }

  const std::string version_text = filename.substr(0, separator_position);

  if (!std::all_of(version_text.begin(),
                   version_text.end(),
                   [](unsigned char character) { return std::isdigit(character) != 0; }))
  {
    throw std::runtime_error("Invalid migration filename: " + filename);
  }

  try
  {
    std::size_t parsed_characters = 0;
    const long version = std::stol(version_text, &parsed_characters);

    if (parsed_characters != version_text.size() || version <= 0 ||
        version > std::numeric_limits<int>::max())
    {
      throw std::runtime_error("Invalid migration filename: " + filename);
    }

    return static_cast<int>(version);
  }
  catch (const std::invalid_argument&)
  {
    throw std::runtime_error("Invalid migration filename: " + filename);
  }
  catch (const std::out_of_range&)
  {
    throw std::runtime_error("Invalid migration filename: " + filename);
  }
}

} // namespace Storage
