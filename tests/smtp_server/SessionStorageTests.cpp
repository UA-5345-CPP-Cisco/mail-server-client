#include <cstdio>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include "logger/Logger.h"
#include "smtp/AuthService.hpp"
#include "smtp/Session.hpp"
#include "smtp/SocketsManager.hpp"
#include "storage/Database.h"
#include "storage/MailMessageRepository.h"
#include "storage/MessageRecipientRepository.h"
#include "storage/MigrationRunner.h"

namespace {

class TestSocketsManager final : public smtp::ISocketsManager
{
  public:
  void Start(const smtp::ServerConfig&) override
  {
  }

  std::vector<smtp::SmtpEvent> PollEvents() override
  {
    return {};
  }

  void Send(smtp::ConnectionId, std::string_view message) override
  {
    responses.emplace_back(message);
  }

  void Close(smtp::ConnectionId) override
  {
  }

  void StartTls(smtp::ConnectionId) override
  {
  }

  void Continue(smtp::ConnectionId) override
  {
  }

  void Stop() override
  {
  }

  std::vector<std::string> responses;
};

class TestLogger final : public Logging::ILogger
{
  public:
  void Log(Logging::LogLevel, std::string_view) override
  {
  }
};

class SessionStorageTest : public testing::Test
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

  void Send(smtp::SmtpSessionHandler& handler,
            smtp::SmtpSessionState& state,
            smtp::SmtpSessionContext& context,
            const std::string& line)
  {
    handler.HandleEvent(
      {smtp::SmtpEventType::MessageReceived, connectionId_, line}, state, context);
  }

  const std::string databasePath_{"smtp-session-storage-test.db"};
  const smtp::ConnectionId connectionId_{1};
};

TEST_F(SessionStorageTest, StoresAcceptedMessageAndQueuesRecipients)
{
  Storage::Database database(databasePath_);
  Storage::MigrationRunner migrations(database, SMTP_SERVER_TEST_MIGRATIONS_DIR);
  migrations.Run();

  Storage::MailMessageRepository mailMessages(database);
  Storage::MessageRecipientRepository messageRecipients(database);
  std::mutex storageMutex;
  smtp::ServerConfig config;
  TestSocketsManager socketsManager;
  smtp::AuthService authService;
  TestLogger logger;
  smtp::SmtpSessionContext context{config,
                                   socketsManager,
                                   authService,
                                   database,
                                   mailMessages,
                                   messageRecipients,
                                   storageMutex,
                                   logger};
  smtp::SmtpSessionState state(connectionId_);
  smtp::SmtpSessionHandler handler;

  Send(handler, state, context, "HELO client.test");
  Send(handler, state, context, "MAIL FROM:<sender@example.com>");
  Send(handler, state, context, "RCPT TO:<first@example.com>");
  Send(handler, state, context, "RCPT TO:<second@example.com>");
  Send(handler, state, context, "DATA");
  Send(handler, state, context, "Message body");
  Send(handler, state, context, ".");

  const std::vector<Storage::MailMessageRecord> queuedMessages =
    mailMessages.FindByStatus(Storage::MailMessageStatus::Queued, 10);
  ASSERT_EQ(queuedMessages.size(), 1);
  EXPECT_EQ(queuedMessages.front().sender_email, "sender@example.com");
  EXPECT_EQ(queuedMessages.front().body, "Message body\r\n");

  const std::vector<Storage::MessageRecipientRecord> recipients =
    messageRecipients.FindByMessageId(queuedMessages.front().id);
  ASSERT_EQ(recipients.size(), 2);
  EXPECT_EQ(recipients.front().delivery_status, Storage::DeliveryStatus::Queued);
  EXPECT_EQ(recipients.back().delivery_status, Storage::DeliveryStatus::Queued);
}

} // namespace
