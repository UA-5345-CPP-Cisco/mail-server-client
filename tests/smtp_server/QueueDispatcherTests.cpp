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
#include "../../libs/mail-storage/include/mail_storage/Database.h"
#include "../../libs/mail-storage/include/mail_storage/MailMessageRepository.h"
#include "../../libs/mail-storage/include/mail_storage/MessageRecipientRepository.h"
#include "../../libs/mail-storage/include/mail_storage/MigrationRunner.h"
#include "../../libs/mail-storage/include/mail_storage/UserRepository.h"
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
  static std::string PrepareDatabasePath(const std::string& path)
  {
    std::remove(path.c_str());
    std::remove((path + "-wal").c_str());
    std::remove((path + "-shm").c_str());
    return path;
  }

  void SetUp() override
  {
    Storage::MigrationRunner migrations(m_database, SMTP_SERVER_TEST_MIGRATIONS_DIR);
    migrations.Run();
  }

  void TearDown() override
  {
    RemoveDatabase();
  }

  void RemoveDatabase() const
  {
    std::remove(m_database_path.c_str());
    std::remove((m_database_path + "-wal").c_str());
    std::remove((m_database_path + "-shm").c_str());
  }

  const std::string m_database_path{"queue-dispatcher-test.db"};

  Storage::Database m_database{PrepareDatabasePath(m_database_path)};
  Storage::UserRepository m_users{m_database};
  Storage::MailMessageRepository m_mail_messages{m_database};
  Storage::MessageRecipientRepository m_message_recipients{m_database};

  std::mutex m_storage_mutex;
  TestLogger m_logger;
  InlineThreadPool m_thread_pool;
};


TEST_F(QueueDispatcherTest, DeliversToActiveLocalUsersAndIgnoresUnknownUsers)
{
  m_users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t messageId = m_mail_messages.CreateMessage(
    std::nullopt, "sender@example.test", std::nullopt, "message body", std::nullopt, false);
  m_message_recipients.CreateRecipient(
    messageId, "local@example.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);
  m_message_recipients.CreateRecipient(
    messageId, "remote@external.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_recipients, m_storage_mutex, m_logger);

  dispatcher.Poll();

  const auto message = m_mail_messages.FindById(messageId);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->status, Storage::MailMessageStatus::Sent);

  const auto recipients = m_message_recipients.FindByMessageId(messageId);
  ASSERT_EQ(recipients.size(), 2);
  EXPECT_EQ(recipients.front().delivery_status, Storage::DeliveryStatus::Delivered);
  EXPECT_EQ(recipients.back().delivery_status, Storage::DeliveryStatus::Failed);
  EXPECT_EQ(m_thread_pool.taskCount, 2);
}

TEST_F(QueueDispatcherTest, FailsMessageWhenNoRecipientIsLocal)
{
  const std::int64_t messageId = m_mail_messages.CreateMessage(
    std::nullopt, "sender@example.test", std::nullopt, "message body", std::nullopt, false);
  m_message_recipients.CreateRecipient(
    messageId, "remote@external.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_recipients, m_storage_mutex, m_logger);

  dispatcher.Poll();

  const auto message = m_mail_messages.FindById(messageId);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->status, Storage::MailMessageStatus::Failed);
}


TEST_F(QueueDispatcherTest, SkipPollWhenIntervalBig)
{
  m_users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t messageId = m_mail_messages.CreateMessage(
    std::nullopt, "sender@example.test", std::nullopt, "message body", std::nullopt, false);

  for (int i = 0; i < 5; ++i)
  {
    m_message_recipients.CreateRecipient(messageId,
                                        "local@example.test",
                                        Storage::RecipientType::To,
                                        Storage::DeliveryStatus::Queued);
  }

  smtp::QueueDispatcher dispatcher(
  {1, 1000000}, m_thread_pool, m_users, m_mail_messages, m_message_recipients, m_storage_mutex, m_logger);

  dispatcher.Poll();

  int taskCountBefore = m_thread_pool.taskCount;

  dispatcher.Poll();

  EXPECT_EQ(taskCountBefore, m_thread_pool.taskCount);
}

TEST_F(QueueDispatcherTest, BastchSizeLimitsPerPoll)
{
  m_users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t messageId = m_mail_messages.CreateMessage(
    std::nullopt, "sender@example.test", std::nullopt, "message body", std::nullopt, false);

  for (int i = 0; i < 5; ++i)
  {
    m_message_recipients.CreateRecipient(messageId,
                                        "local@example.test",
                                        Storage::RecipientType::To,
                                        Storage::DeliveryStatus::Queued);
  }

  smtp::QueueDispatcher dispatcher(
  {2, 100}, m_thread_pool, m_users, m_mail_messages, m_message_recipients, m_storage_mutex, m_logger);

  dispatcher.Poll();

  EXPECT_EQ(m_thread_pool.taskCount, 2);

  const auto recipients = m_message_recipients.FindByMessageId(messageId);
  const auto stillQueued =
    std::count_if(recipients.begin(), recipients.end(), [](const auto& recipient) {
      return recipient.delivery_status == Storage::DeliveryStatus::Queued;
    });
  EXPECT_EQ(stillQueued, 3);
}

TEST_F(QueueDispatcherTest, MultipleMessagesInSinglePoll)
{
  m_users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t message_id = m_mail_messages.CreateMessage(
    std::nullopt, "sender-one@example.test", std::nullopt, "first body", std::nullopt, false);
  m_message_recipients.CreateRecipient(message_id,
                                      "local@example.test",
                                      Storage::RecipientType::To,
                                      Storage::DeliveryStatus::Queued);

  const std::int64_t message_id1 = m_mail_messages.CreateMessage(
    std::nullopt, "sender-two@example.test", std::nullopt, "second body", std::nullopt, false);
  m_message_recipients.CreateRecipient(message_id1,
                                      "local@example.test",
                                      Storage::RecipientType::To,
                                      Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_recipients, m_storage_mutex, m_logger);

  dispatcher.Poll();

  const auto first_message = m_mail_messages.FindById(message_id);
  const auto second_message = m_mail_messages.FindById(message_id);
  ASSERT_TRUE(first_message.has_value());
  ASSERT_TRUE(second_message.has_value());
  EXPECT_EQ(first_message->status, Storage::MailMessageStatus::Sent);
  EXPECT_EQ(second_message->status, Storage::MailMessageStatus::Sent);
  EXPECT_EQ(m_thread_pool.taskCount, 2);
}


TEST_F(QueueDispatcherTest, RecipientsFailedWhenNoUsersExist)
{
  const std::int64_t message_id = m_mail_messages.CreateMessage(
    std::nullopt, "sender@example.test", std::nullopt, "message body", std::nullopt, false);
  m_message_recipients.CreateRecipient(
    message_id, "ghost@example.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_recipients, m_storage_mutex, m_logger);

  EXPECT_NO_THROW(dispatcher.Poll());

  const auto message = m_mail_messages.FindById(message_id);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->status, Storage::MailMessageStatus::Failed);

  const auto recipients = m_message_recipients.FindByMessageId(message_id);
  ASSERT_EQ(recipients.size(), 1);
  EXPECT_EQ(recipients.front().delivery_status, Storage::DeliveryStatus::Failed);
}

TEST_F(QueueDispatcherTest, HandlesMixedLocalAndRemoteRecipientsForSameMessage)
{
  m_users.CreateUser("user-a", "user-a@example.test", "password-hash");

  const std::int64_t messageId = m_mail_messages.CreateMessage(
    std::nullopt, "sender@example.test", std::nullopt, "message body", std::nullopt, false);
  m_message_recipients.CreateRecipient(
    messageId, "user-a@example.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);
  m_message_recipients.CreateRecipient(
    messageId, "remote@external.test", Storage::RecipientType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_recipients, m_storage_mutex, m_logger);

  dispatcher.Poll();

  const auto message = m_mail_messages.FindById(messageId);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->status, Storage::MailMessageStatus::Sent);
  EXPECT_EQ(m_thread_pool.taskCount, 2);
}

} // namespace
