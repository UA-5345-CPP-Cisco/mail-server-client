#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include "mail_storage/Database.h"
#include "mail_storage/Statement.h"

namespace {

class StatementTest : public testing::Test
{
  protected:
  void SetUp() override
  {
    const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();

    m_database_path =
      std::filesystem::temp_directory_path() / ("mail_statement_test_" + std::to_string(timestamp) + ".sqlite3");

    m_database = std::make_unique<Storage::Database>(m_database_path);
    m_database->Execute(
      R"SQL(
				CREATE TABLE statement_values (
					id INTEGER PRIMARY KEY AUTOINCREMENT,
					number INTEGER NOT NULL,
					text_value TEXT NOT NULL,
					nullable_value TEXT
				);
			)SQL");
  }

  void TearDown() override
  {
    m_database.reset();

    std::error_code error_code;
    std::filesystem::remove(m_database_path, error_code);
    std::filesystem::remove(m_database_path.string() + "-shm", error_code);
    std::filesystem::remove(m_database_path.string() + "-wal", error_code);
  }

  std::filesystem::path m_database_path;
  std::unique_ptr<Storage::Database> m_database;
};

TEST_F(StatementTest, BindsAndReadsValues)
{
  Storage::Statement insert_statement(*m_database,
                                      R"SQL(
			INSERT INTO statement_values (
				number,
				text_value,
				nullable_value
			)
			VALUES (?, ?, ?);
		)SQL");

  insert_statement.BindInt64(1, 9000000000);
  insert_statement.BindText(2, "value'); DROP TABLE statement_values; --");
  insert_statement.BindNull(3);
  EXPECT_FALSE(insert_statement.Step());

  const std::int64_t inserted_id = insert_statement.LastInsertRowId();
  EXPECT_GT(inserted_id, 0);
  EXPECT_EQ(insert_statement.ChangedRowCount(), 1);

  Storage::Statement select_statement(*m_database,
                                      R"SQL(
			SELECT
				id,
				number,
				text_value,
				nullable_value
			FROM statement_values
			WHERE id = ?;
		)SQL");

  select_statement.BindInt64(1, inserted_id);
  ASSERT_TRUE(select_statement.Step());
  EXPECT_EQ(select_statement.ColumnInt64(0), inserted_id);
  EXPECT_EQ(select_statement.ColumnInt64(1), 9000000000);
  EXPECT_EQ(select_statement.ColumnText(2), "value'); DROP TABLE statement_values; --");
  EXPECT_TRUE(select_statement.ColumnIsNull(3));
  EXPECT_FALSE(select_statement.Step());
}

TEST_F(StatementTest, ThrowsWhenStatementCannotBePrepared)
{
  EXPECT_THROW(Storage::Statement statement(*m_database, "INVALID SQL"), std::runtime_error);
}

} // namespace
