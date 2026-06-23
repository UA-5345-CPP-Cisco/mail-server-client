#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

#include "storage/Database.h"
#include "storage/MigrationRunner.h"

namespace { 

class MigrationRunnerTest : public testing::Test
{
protected:
	void SetUp() override
	{
		const auto timestamp =
			std::chrono::steady_clock::now().time_since_epoch().count();
		const std::string test_name = std::to_string(timestamp);

		m_database_path =
			std::filesystem::temp_directory_path() /
			("mail_migration_test_" + test_name + ".sqlite3");
		m_migrations_path =
			std::filesystem::temp_directory_path() /
			("mail_migrations_" + test_name);

		std::filesystem::create_directories(m_migrations_path);
	}

	void TearDown() override
	{
		std::error_code error_code;
		std::filesystem::remove_all(m_migrations_path, error_code);
		std::filesystem::remove(m_database_path, error_code);
		std::filesystem::remove(m_database_path.string() + "-shm", error_code);
		std::filesystem::remove(m_database_path.string() + "-wal", error_code);
	}

	void WriteMigration(
		const std::string& filename,
		const std::string& sql
	)
	{
		std::ofstream file(m_migrations_path / filename);

		if (!file.is_open())
		{
			throw std::runtime_error("Failed to create test migration");
		}

		file << sql;

		if (!file)
		{
			throw std::runtime_error("Failed to write test migration");
		}
	}

	std::filesystem::path m_database_path;
	std::filesystem::path m_migrations_path;
};

TEST_F(MigrationRunnerTest, AppliesInitialUserSchema)
{
	Storage::Database database(m_database_path);
	Storage::MigrationRunner migration_runner(
		database,
		STORAGE_TEST_MIGRATIONS_DIR
	);

	migration_runner.Run();

	EXPECT_NO_THROW(
		database.Execute(
			R"SQL(
				INSERT INTO users (
					username,
					email,
					password_hash
				)
				VALUES (
					'alice',
					'alice@example.com',
					'hash'
				);

				INSERT INTO users (
					username,
					email,
					password_hash,
					status
				)
				VALUES (
					'bob',
					'bob@example.com',
					'hash',
					'disabled'
				);
			)SQL"
		)
	);

	EXPECT_THROW(
		database.Execute(
			R"SQL(
				INSERT INTO users (
					username,
					email,
					password_hash,
					status
				)
				VALUES (
					'charlie',
					'charlie@example.com',
					'hash',
					'pending'
				);
			)SQL"
		),
		std::runtime_error
	);
}

TEST_F(MigrationRunnerTest, AppliesEachMigrationOnlyOnce)
{
	Storage::Database database(m_database_path);
	Storage::MigrationRunner migration_runner(
		database,
		STORAGE_TEST_MIGRATIONS_DIR
	);

	EXPECT_NO_THROW(migration_runner.Run());
	EXPECT_NO_THROW(migration_runner.Run());
}

TEST_F(MigrationRunnerTest, AppliesMigrationsByNumericVersion)
{
	WriteMigration(
		"010_insert_value.sql",
		"INSERT INTO migration_order (value) VALUES ('created');"
	);
	WriteMigration(
		"002_create_table.sql",
		R"SQL(
			CREATE TABLE migration_order (
				value TEXT NOT NULL UNIQUE
			);
		)SQL"
	);

	Storage::Database database(m_database_path);
	Storage::MigrationRunner migration_runner(database, m_migrations_path);

	EXPECT_NO_THROW(migration_runner.Run());
	EXPECT_THROW(
		database.Execute(
			"INSERT INTO migration_order (value) VALUES ('created');"
		),
		std::runtime_error
	);
}

TEST_F(MigrationRunnerTest, RejectsDuplicateMigrationVersions)
{
	WriteMigration("001_first.sql", "CREATE TABLE first_table (id INTEGER);");
	WriteMigration("001_second.sql", "CREATE TABLE second_table (id INTEGER);");

	Storage::Database database(m_database_path);
	Storage::MigrationRunner migration_runner(database, m_migrations_path);

	EXPECT_THROW(migration_runner.Run(), std::runtime_error);
}

TEST_F(MigrationRunnerTest, RollsBackFailedMigration)
{
	WriteMigration(
		"001_failed.sql",
		R"SQL(
			CREATE TABLE rollback_check (
				id INTEGER PRIMARY KEY
			);

			INVALID SQL;
		)SQL"
	);

	Storage::Database database(m_database_path);
	Storage::MigrationRunner migration_runner(database, m_migrations_path);

	EXPECT_THROW(migration_runner.Run(), std::runtime_error);
	EXPECT_NO_THROW(
		database.Execute(
			"CREATE TABLE rollback_check (id INTEGER PRIMARY KEY);"
		)
	);
}

TEST_F(MigrationRunnerTest, RejectsInvalidMigrationFilename)
{
	WriteMigration("invalid.sql", "CREATE TABLE invalid_name (id INTEGER);");

	Storage::Database database(m_database_path);
	Storage::MigrationRunner migration_runner(database, m_migrations_path);

	EXPECT_THROW(migration_runner.Run(), std::runtime_error);
}

}
