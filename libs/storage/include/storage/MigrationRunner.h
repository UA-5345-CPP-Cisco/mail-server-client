#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include "storage/Database.h"

namespace Storage {

class MigrationRunner
{
  public:
  MigrationRunner(Database& database, const std::filesystem::path& migrations_path);

  void Run();

  private:
  struct Migration
  {
    int version;
    std::string name;
    std::filesystem::path path;
  };

  Database& m_database;
  std::filesystem::path m_migrations_path;

  void EnsureMigrationsTable();
  std::vector<Migration> LoadMigrations() const;
  void ApplyMigration(const Migration& migration);
  bool IsMigrationApplied(int version) const;
  void RecordMigration(const Migration& migration);
  std::string ReadMigration(const std::filesystem::path& path) const;
  int ParseVersion(const std::string& filename) const;
};

} // namespace Storage
