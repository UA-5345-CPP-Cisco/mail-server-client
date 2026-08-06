#include <cstdio>
#include <exception>
#include <functional>
#include <future>
#include <mutex>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include "logger/Logger.h"
#include "mail_storage/Database.h"
#include "mail_storage/MailMessageActorRepository.h"
#include "mail_storage/MailMessageRepository.h"
#include "mail_storage/MigrationRunner.h"
#include "mail_storage/UserRepository.h"
#include "smtp/QueueDispatcher.hpp"
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

  std::int64_t CreateQueuedMessage(const std::string& sender_email = "sender@example.test",
                                   const std::string& body = "message body")
  {
    const std::int64_t message_id = m_mail_messages.CreateMessage(std::nullopt, body, std::nullopt);
    m_message_actors.CreateActor(message_id, sender_email, Storage::MailMessageActorType::From);
    return message_id;
  }

  const std::string m_database_path{"queue-dispatcher-test.db"};

  Storage::Database m_database{PrepareDatabasePath(m_database_path)};
  Storage::UserRepository m_users{m_database};
  Storage::MailMessageRepository m_mail_messages{m_database};
  Storage::MailMessageActorRepository m_message_actors{m_database};

  std::mutex m_storage_mutex;
  TestLogger m_logger;
  InlineThreadPool m_thread_pool;
};

TEST_F(QueueDispatcherTest, DeliversToActiveLocalUsersAndIgnoresUnknownUsers)
{
  m_users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t messageId = CreateQueuedMessage();
  m_message_actors.CreateActor(
    messageId, "local@example.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);
  m_message_actors.CreateActor(
    messageId, "remote@external.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_actors, m_storage_mutex, m_logger);

  dispatcher.Poll();

  const auto message = m_mail_messages.FindById(messageId);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->message_status, Storage::MailMessageStatus::Sent);

  const auto recipients = m_message_actors.FindByMessageId(messageId);
  ASSERT_EQ(recipients.size(), 3);
  EXPECT_FALSE(recipients[0].delivery_status.has_value());
  EXPECT_EQ(recipients[1].delivery_status, Storage::DeliveryStatus::Delivered);
  EXPECT_EQ(recipients[2].delivery_status, Storage::DeliveryStatus::Failed);
  EXPECT_EQ(m_thread_pool.taskCount, 2);
}

TEST_F(QueueDispatcherTest, FailsMessageWhenNoRecipientIsLocal)
{
  const std::int64_t messageId = CreateQueuedMessage();
  m_message_actors.CreateActor(
    messageId, "remote@external.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_actors, m_storage_mutex, m_logger);

  dispatcher.Poll();

  const auto message = m_mail_messages.FindById(messageId);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->message_status, Storage::MailMessageStatus::Failed);
}

TEST_F(QueueDispatcherTest, SkipPollWhenIntervalBig)
{
  m_users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t messageId = CreateQueuedMessage();

  for (int i = 0; i < 5; ++i)
  {
    m_message_actors.CreateActor(
      messageId, "local@example.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);
  }

  smtp::QueueDispatcher dispatcher(
    {1, 1000000}, m_thread_pool, m_users, m_mail_messages, m_message_actors, m_storage_mutex, m_logger);

  dispatcher.Poll();

  int taskCountBefore = m_thread_pool.taskCount;

  dispatcher.Poll();

  EXPECT_EQ(taskCountBefore, m_thread_pool.taskCount);
}

TEST_F(QueueDispatcherTest, BatchSizeLimitsPerPoll)
{
  m_users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t messageId = CreateQueuedMessage();

  for (int i = 0; i < 5; ++i)
  {
    m_message_actors.CreateActor(
      messageId, "local@example.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);
  }

  smtp::QueueDispatcher dispatcher(
    {2, 100}, m_thread_pool, m_users, m_mail_messages, m_message_actors, m_storage_mutex, m_logger);

  dispatcher.Poll();

  EXPECT_EQ(m_thread_pool.taskCount, 2);

  const auto recipients = m_message_actors.FindByMessageId(messageId);
  const auto stillQueued =
    std::count_if(recipients.begin(),
                  recipients.end(),
                  [](const auto& recipient) { return recipient.delivery_status == Storage::DeliveryStatus::Queued; });
  EXPECT_EQ(stillQueued, 3);
}

TEST_F(QueueDispatcherTest, MultipleMessagesInSinglePoll)
{
  m_users.CreateUser("local-user", "local@example.test", "password-hash");

  const std::int64_t message_id = CreateQueuedMessage("sender-one@example.test", "first body");
  m_message_actors.CreateActor(
    message_id, "local@example.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);

  const std::int64_t message_id1 = CreateQueuedMessage("sender-two@example.test", "second body");
  m_message_actors.CreateActor(
    message_id1, "local@example.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_actors, m_storage_mutex, m_logger);

  dispatcher.Poll();

  const auto first_message = m_mail_messages.FindById(message_id);
  const auto second_message = m_mail_messages.FindById(message_id1);
  ASSERT_TRUE(first_message.has_value());
  ASSERT_TRUE(second_message.has_value());
  EXPECT_EQ(first_message->message_status, Storage::MailMessageStatus::Sent);
  EXPECT_EQ(second_message->message_status, Storage::MailMessageStatus::Sent);
  EXPECT_EQ(m_thread_pool.taskCount, 2);
}

TEST_F(QueueDispatcherTest, RecipientsFailedWhenNoUsersExist)
{
  const std::int64_t message_id = CreateQueuedMessage();
  m_message_actors.CreateActor(
    message_id, "ghost@example.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_actors, m_storage_mutex, m_logger);

  EXPECT_NO_THROW(dispatcher.Poll());

  const auto message = m_mail_messages.FindById(message_id);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->message_status, Storage::MailMessageStatus::Failed);

  const auto recipients = m_message_actors.FindByMessageId(message_id);
  ASSERT_EQ(recipients.size(), 2);
  EXPECT_EQ(recipients.back().delivery_status, Storage::DeliveryStatus::Failed);
}

TEST_F(QueueDispatcherTest, HandlesMixedLocalAndRemoteRecipientsForSameMessage)
{
  m_users.CreateUser("user-a", "user-a@example.test", "password-hash");

  const std::int64_t messageId = CreateQueuedMessage();
  m_message_actors.CreateActor(
    messageId, "user-a@example.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);
  m_message_actors.CreateActor(
    messageId, "remote@external.test", Storage::MailMessageActorType::To, Storage::DeliveryStatus::Queued);

  smtp::QueueDispatcher dispatcher(
    {32, 100}, m_thread_pool, m_users, m_mail_messages, m_message_actors, m_storage_mutex, m_logger);

  dispatcher.Poll();

  const auto message = m_mail_messages.FindById(messageId);
  ASSERT_TRUE(message.has_value());
  EXPECT_EQ(message->message_status, Storage::MailMessageStatus::Sent);
  EXPECT_EQ(m_thread_pool.taskCount, 2);
}

} // namespace
