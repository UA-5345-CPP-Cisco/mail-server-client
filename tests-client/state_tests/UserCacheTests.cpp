#include <memory>

#include <QTemporaryFile>
#include <gtest/gtest.h>

#include "database/CacheStore.h"
#include "database/DatabaseManager.h"
#include "users/UserCache.h"

class UserCacheTests : public ::testing::Test
{
  protected:
  std::unique_ptr<ISXCurrentUser::UserCache> user_cache;
  std::unique_ptr<Storage::CacheStore> store;
  std::unique_ptr<Storage::Database> db;

  void SetUp() override
  {
    ISXDatabaseManager::DatabaseManager::EnsureInitialized();
    this->db =
      std::make_unique<Storage::Database>(ISXDatabaseManager::DatabaseManager::DatabasePath());
    this->store = std::make_unique<Storage::CacheStore>(*this->db);
    this->user_cache = std::make_unique<ISXCurrentUser::UserCache>(*this->store);
  }

  void TearDown() override
  {
    user_cache.reset();
    store.reset();
    db.reset();
  }
};

TEST_F(UserCacheTests, TestSaveLoad)
{
  const ISXCurrentUser::CachedUserProfile profile = {
    .username = "username",
    .email = "Some email",
    .avatar_path = "Some path",
  };
  this->user_cache->Save(profile);
  auto res = user_cache->Load();
  ASSERT_TRUE(res.has_value());
  EXPECT_EQ("username", res->username);
  EXPECT_EQ("Some email", res->email);
  EXPECT_EQ("Some path", res->avatar_path);
}

TEST_F(UserCacheTests, TestInvalidate)
{
  const ISXCurrentUser::CachedUserProfile profile = {
    .username = "username",
    .email = "Some email",
    .avatar_path = "Some path",
  };
  this->user_cache->Save(profile);
  this->user_cache->Invalidate();
  auto res = user_cache->Load();
  ASSERT_FALSE(res.has_value());
}

TEST_F(UserCacheTests, ThrowsExceptionOnCorruptedData)
{
  store->Put(
    "user-profile", "active-user", "definitely_not_a_valid_json", std::chrono::minutes(10), 1);
  EXPECT_THROW(this->user_cache->Load(), std::runtime_error);
}