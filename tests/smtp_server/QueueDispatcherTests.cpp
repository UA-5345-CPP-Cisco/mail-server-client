#include <cstdio>
#include <exception>
#include <functional>
#include <future>
#include <mutex>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include "logger/Logger.h"
#include "smtp/QueueDispatcher.hpp"
#include "storage/Database.h"
#include "storage/MailMessageRepository.h"
#include "storage/MessageRecipientRepository.h"
#include "storage/MigrationRunner.h"
#include "storage/UserRepository.h"
#include "thread_pool/ThreadPool.h"

namespace {

class InlineThreadPool final : public Concurrency::IThreadPool
{
  public:
  std::future<void> Enqueue(std::function<void()> task) override
  {
    std::promise<void> promise;
    std::future<void> result = promise.get_future();

    try
    {
      task();
      promise.set_value();
    }
    catch (...)
    {
      promise.set_exception(std::current_exception());
    }

    ++taskCount;
    return result;
  }

  std::size_t taskCount{0};
};

class TestLogger final : public Logging::ILogger
{
  public:
  void Log(Logging::LogLevel, std::string_view) override
  {
  }
};

class QueueDispatcherTest : public testing::Test
{
  protected:
  void SetUp() override
  {
    RemoveDatabase();
  }

  void TearDown() override
  {
    RemoveDatabase();
  }

  void RemoveDatabase() const
  {
    std::remove(databasePath_.c_str());
    std::remove((databasePath_ + "-wal").c_str());
    std::remove((databasePath_ + "-shm").c_str());
  }

  const std::string databasePath_{"queue-dispatcher-test.db"};
};

TEST_F(QueueDispatcherTest, DeliversToActiveLocalUsersAndIgnoresUnknownUsers)
{
  Storage::Database database(databasePath_);
  Storage::MigrationRunner migrations(database, SMTP_SERVER_TEST_MIGRATIONS_DIR);
  migrations.Run();

  Storage::UserRepository users(database);
  Storage::MailMessageRepository mailMessages(database);
  Storage::MessageRecipientRepository messageRecipients(database);
  users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t messageId = mailMessages.CreateMessage(
    std::nullopt, "sender@example.test", std::nullopt, "message body", std::nullopt);
  messageRecipients.CreateRecipient(
    messageId, "local@example.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);
  messageRecipients.CreateRecipient(
    messageId, "remote@external.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);

  InlineThreadPool threadPool;
  TestLogger logger;
  std::mutex storageMutex;
  smtp::QueueDispatcher dispatcher(
    {32, 100}, threadPool, users, mailMessages, messageRecipients, storageMutex, logger);

  dispatcher.Poll();

  const auto message = mailMessages.FindById(messageId);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->status, Storage::MailMessageStatus::Sent);

  const auto recipients = messageRecipients.FindByMessageId(messageId);
  ASSERT_EQ(recipients.size(), 2);
  EXPECT_EQ(recipients.front().delivery_status, Storage::DeliveryStatus::Delivered);
  EXPECT_EQ(recipients.back().delivery_status, Storage::DeliveryStatus::Failed);
  EXPECT_EQ(threadPool.taskCount, 2);
}

TEST_F(QueueDispatcherTest, FailsMessageWhenNoRecipientIsLocal)
{
  Storage::Database database(databasePath_);
  Storage::MigrationRunner migrations(database, SMTP_SERVER_TEST_MIGRATIONS_DIR);
  migrations.Run();

  Storage::UserRepository users(database);
  Storage::MailMessageRepository mailMessages(database);
  Storage::MessageRecipientRepository messageRecipients(database);
  const std::int64_t messageId = mailMessages.CreateMessage(
    std::nullopt, "sender@example.test", std::nullopt, "message body", std::nullopt);
  messageRecipients.CreateRecipient(
    messageId, "remote@external.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);

  InlineThreadPool threadPool;
  TestLogger logger;
  std::mutex storageMutex;
  smtp::QueueDispatcher dispatcher(
    {32, 100}, threadPool, users, mailMessages, messageRecipients, storageMutex, logger);

  dispatcher.Poll();

  const auto message = mailMessages.FindById(messageId);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->status, Storage::MailMessageStatus::Failed);
}

} // namespace
