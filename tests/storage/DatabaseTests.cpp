#include <chrono>
#include <filesystem>
#include <string>
#include <utility>

#include <gtest/gtest.h>

#include "storage/Database.h"

namespace {

class DatabaseTest : public testing::Test
{
  protected:
  void SetUp() override
  {
    const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();

    m_database_path = std::filesystem::temp_directory_path() /
                      ("mail_database_test_" + std::to_string(timestamp) + ".sqlite3");
  }

  void TearDown() override
  {
    std::error_code error_code;
    std::filesystem::remove(m_database_path, error_code);
    std::filesystem::remove(m_database_path.string() + "-shm", error_code);
    std::filesystem::remove(m_database_path.string() + "-wal", error_code);
  }

  std::filesystem::path m_database_path;
};

TEST_F(DatabaseTest, OpensDatabaseAndExecutesSql)
{
  Storage::Database database(m_database_path);

  EXPECT_NO_THROW(database.Execute(
    R"SQL(
				CREATE TABLE users (
					id INTEGER PRIMARY KEY,
					email TEXT NOT NULL UNIQUE
				);

				INSERT INTO users (email)
				VALUES ('user@example.com');
			)SQL"));

  EXPECT_TRUE(std::filesystem::exists(m_database_path));
}

TEST_F(DatabaseTest, ThrowsWhenSqlExecutionFails)
{
  Storage::Database database(m_database_path);

  EXPECT_THROW(database.Execute("CREATE TABLE invalid SQL;"), std::runtime_error);
}

TEST_F(DatabaseTest, EnablesForeignKeyConstraints)
{
  Storage::Database database(m_database_path);

  database.Execute(
    R"SQL(
			CREATE TABLE parent_records (
				id INTEGER PRIMARY KEY
			);

			CREATE TABLE child_records (
				id INTEGER PRIMARY KEY,
				parent_id INTEGER NOT NULL,
				FOREIGN KEY (parent_id) REFERENCES parent_records(id)
			);
		)SQL");

  EXPECT_THROW(database.Execute("INSERT INTO child_records (parent_id) VALUES (42);"),
               std::runtime_error);
}

TEST_F(DatabaseTest, ClosesConnectionAndPreservesData)
{
  {
    Storage::Database database(m_database_path);
    database.Execute(
      R"SQL(
				CREATE TABLE settings (
					name TEXT NOT NULL UNIQUE
				);

				INSERT INTO settings (name)
				VALUES ('smtp_port');
			)SQL");
  }

  Storage::Database reopened_database(m_database_path);

  EXPECT_THROW(reopened_database.Execute("INSERT INTO settings (name) VALUES ('smtp_port');"),
               std::runtime_error);
}

TEST_F(DatabaseTest, TransfersConnectionOwnershipWhenMoved)
{
  Storage::Database database(m_database_path);
  Storage::Database moved_database(std::move(database));

  EXPECT_NO_THROW(
    moved_database.Execute("CREATE TABLE moved_connection (id INTEGER PRIMARY KEY);"));
}

TEST_F(DatabaseTest, ThrowsWhenDatabaseCannotBeOpened)
{
  std::filesystem::path missing_directory = m_database_path;
  missing_directory += ".missing";

  const std::filesystem::path invalid_path = missing_directory / "database.sqlite3";

  EXPECT_THROW(Storage::Database database(invalid_path), std::runtime_error);
}

} // namespace
