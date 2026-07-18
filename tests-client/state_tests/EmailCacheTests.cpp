#include <memory>

#include <QTemporaryFile>
#include <gtest/gtest.h>

#include "database/CacheStore.h"
#include "database/DatabaseManager.h"
#include "mail/EmailCache.h"

class EmailCacheTests : public ::testing::Test
{
  protected:
  std::unique_ptr<ISXMail::EmailListCache> cache;
  std::unique_ptr<Storage::CacheStore> store;
  std::unique_ptr<Storage::Database> db;

  void SetUp() override
  {
    ISXDatabaseManager::DatabaseManager::EnsureInitialized();
    this->db =
      std::make_unique<Storage::Database>(ISXDatabaseManager::DatabaseManager::DatabasePath());
    this->store = std::make_unique<Storage::CacheStore>(*this->db);
    this->cache = std::make_unique<ISXMail::EmailListCache>(*this->store);
    this->cache->InvalidateAll();
  }

  void TearDown() override
  {
    cache.reset();
    store.reset();
    db.reset();
  }
};

TEST_F(EmailCacheTests, TestSaveAndLoad)
{
  const QString folder = "folder";
  const QString search_test = "some text";
  const QString payload = "payload";
  cache->Save(folder, search_test, payload);
  auto res = cache->Load(folder, search_test);
  ASSERT_TRUE(res.has_value());
  EXPECT_EQ(res->folder, folder);
  EXPECT_EQ(res->payload, payload);
  EXPECT_EQ(res->search_text, search_test);
  EXPECT_EQ(res->version, 1);
}

TEST_F(EmailCacheTests, InvalidateFolder)
{
  cache->Save("inbox", "urgent_text", "payload_1");
  cache->Save("inbox", "spam_text", "payload_2");
  cache->Save("sent", "some_text", "payload_3");
  cache->InvalidateFolder("inbox");
  auto res0 = cache->Load("inbox", "urgent_text");
  EXPECT_FALSE(res0.has_value());
  auto res1 = cache->Load("inbox", "spam_text");
  EXPECT_FALSE(res1.has_value());
  auto res2 = cache->Load("sent", "some_text");
  EXPECT_TRUE(res2.has_value());
}

TEST_F(EmailCacheTests, InvalidateAll)
{
  cache->Save("inbox", "urgent_text", "payload_1");
  cache->Save("inbox", "spam_text", "payload_2");
  cache->Save("sent", "some_text", "payload_3");
  cache->InvalidateAll();
  auto res0 = cache->Load("inbox", "urgent_text");
  EXPECT_FALSE(res0.has_value());
  auto res1 = cache->Load("inbox", "spam_text");
  EXPECT_FALSE(res1.has_value());
  auto res2 = cache->Load("sent", "some_text");
  EXPECT_FALSE(res2.has_value());
}

TEST_F(EmailCacheTests, NormalizesFolderAndSearchTextKeys)
{
  cache->Save("   INBOX   ", "  URGENT  ", "my_payload");
  auto result = cache->Load("inbox", "urgent");
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->payload, "my_payload");
  cache->InvalidateFolder("  InBoX  ");
  EXPECT_FALSE(cache->Load("inbox", "urgent").has_value());
}