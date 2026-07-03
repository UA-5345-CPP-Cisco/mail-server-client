#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include "storage/Database.h"
#include "storage/MigrationRunner.h"
#include "storage/UserRecord.h"
#include "storage/UserRepository.h"

namespace {

class UserRepositoryTest : public testing::Test
{
  protected:
  void SetUp() override
  {
    const auto timestamp = std::chrono::steady_clock::now().time_since_epoch().count();

    m_database_path = std::filesystem::temp_directory_path() /
                      ("mail_user_test_" + std::to_string(timestamp) + ".sqlite3");

    m_database = std::make_unique<Storage::Database>(m_database_path);

    Storage::MigrationRunner migration_runner(*m_database, STORAGE_TEST_MIGRATIONS_DIR);
    migration_runner.Run();

    m_repository = std::make_unique<Storage::UserRepository>(*m_database);
  }

  void TearDown() override
  {
    m_repository.reset();
    m_database.reset();

    std::error_code error_code;
    std::filesystem::remove(m_database_path, error_code);
    std::filesystem::remove(m_database_path.string() + "-shm", error_code);
    std::filesystem::remove(m_database_path.string() + "-wal", error_code);
  }

  std::filesystem::path m_database_path;
  std::unique_ptr<Storage::Database> m_database;
  std::unique_ptr<Storage::UserRepository> m_repository;
};

TEST_F(UserRepositoryTest, CreatesAndFindsUser)
{
  const std::int64_t user_id =
    m_repository->CreateUser("alice", "alice@example.com", "password-hash");

  const std::optional<Storage::UserRecord> user_by_id = m_repository->FindById(user_id);
  const std::optional<Storage::UserRecord> user_by_email =
    m_repository->FindByEmail("alice@example.com");
  const std::optional<Storage::UserRecord> user_by_username = m_repository->FindByUsername("alice");

  ASSERT_TRUE(user_by_id.has_value());
  ASSERT_TRUE(user_by_email.has_value());
  ASSERT_TRUE(user_by_username.has_value());

  EXPECT_EQ(user_by_id->id, user_id);
  EXPECT_EQ(user_by_id->username, "alice");
  EXPECT_EQ(user_by_id->email, "alice@example.com");
  EXPECT_EQ(user_by_id->password_hash, "password-hash");
  EXPECT_EQ(user_by_id->status, Storage::UserStatus::Active);
  EXPECT_FALSE(user_by_id->created_at.empty());
  EXPECT_EQ(user_by_email->id, user_id);
  EXPECT_EQ(user_by_username->id, user_id);
}

TEST_F(UserRepositoryTest, ReturnsEmptyOptionalWhenUserDoesNotExist)
{
  EXPECT_FALSE(m_repository->FindById(1).has_value());
  EXPECT_FALSE(m_repository->FindByEmail("missing@example.com").has_value());
  EXPECT_FALSE(m_repository->FindByUsername("missing").has_value());
}

TEST_F(UserRepositoryTest, UpdatesUserStatus)
{
  const std::int64_t user_id =
    m_repository->CreateUser("alice", "alice@example.com", "password-hash");

  EXPECT_TRUE(m_repository->UpdateStatus(user_id, Storage::UserStatus::Disabled));
  EXPECT_FALSE(m_repository->UpdateStatus(user_id + 1, Storage::UserStatus::Active));

  const std::optional<Storage::UserRecord> user = m_repository->FindById(user_id);

  ASSERT_TRUE(user.has_value());
  EXPECT_EQ(user->status, Storage::UserStatus::Disabled);
}

TEST_F(UserRepositoryTest, RejectsDuplicateUsernameAndEmail)
{
  m_repository->CreateUser("alice", "alice@example.com", "password-hash");

  EXPECT_THROW(m_repository->CreateUser("alice", "other@example.com", "password-hash"),
               std::runtime_error);
  EXPECT_THROW(m_repository->CreateUser("other", "alice@example.com", "password-hash"),
               std::runtime_error);
}

TEST_F(UserRepositoryTest, TreatsInputAsBoundValues)
{
  const std::string username = "user'); DROP TABLE users; --";
  const std::int64_t user_id =
    m_repository->CreateUser(username, "safe@example.com", "password-hash");

  const std::optional<Storage::UserRecord> user = m_repository->FindByUsername(username);

  ASSERT_TRUE(user.has_value());
  EXPECT_EQ(user->id, user_id);

  EXPECT_NO_THROW(m_repository->CreateUser("second-user", "second@example.com", "password-hash"));
}

} // namespace
