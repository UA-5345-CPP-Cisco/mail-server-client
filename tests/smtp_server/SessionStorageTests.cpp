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
#include "../../libs/mail-storage/include/mail_storage/Database.h"
#include "../../libs/mail-storage/include/mail_storage/MailMessageRepository.h"
#include "../../libs/mail-storage/include/mail_storage/MessageRecipientRepository.h"
#include "../../libs/mail-storage/include/mail_storage/MigrationRunner.h"


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
    Storage::MigrationRunner migrations(m_database, SMTP_SERVER_TEST_MIGRATIONS_DIR);
    migrations.Run();
  }

  void TearDown() override
  {
    RemoveDatabase();
  }

  void RemoveDatabase() const
  {
    std::remove(m_databasePath.c_str());
    std::remove((m_databasePath + "-wal").c_str());
    std::remove((m_databasePath + "-shm").c_str());
  }

  void Send(const std::string& line)
  {
    m_handler.HandleEvent(
      {smtp::SmtpEventType::MessageReceived, m_connectionId, line}, m_state, m_context);
  }

  std::string GetLastErrorMessage()
  {
    if (m_socketsManager.responses.empty())
    {
      return {};
    }

    return m_socketsManager.responses.back().substr(0,3);
  }

  std::string PrepareDatabasePath(const std::string& path)
  {
    std::remove(path.c_str());
    std::remove((path + "-wal").c_str());
    std::remove((path + "-shm").c_str());
    return path;
  }

  const std::string m_databasePath{"smtp-session-storage-test.db"};
  const smtp::ConnectionId m_connectionId{1};

  Storage::Database m_database{PrepareDatabasePath(m_databasePath)};
  Storage::MailMessageRepository m_mailMessages{m_database};
  Storage::MessageRecipientRepository m_messageRecipients{m_database};

  std::mutex m_storageMutex;
  smtp::ServerConfig m_config;
  TestSocketsManager m_socketsManager;
  smtp::AuthService m_authService;
  TestLogger m_logger;

  smtp::SmtpSessionContext m_context{
    m_config,
    m_socketsManager,
    m_authService,
    m_database,
    m_mailMessages,
    m_messageRecipients,
    m_storageMutex,
    m_logger};

  smtp::SmtpSessionState m_state{m_connectionId};
  smtp::SmtpSessionHandler m_handler;

  std::shared_ptr<smtp::SmtpSession> m_session;
};

class SmtpSessionQueueTest : public ::testing::Test
{
protected:
  void SetUp()
  {
    Storage::MigrationRunner migrations(m_database, SMTP_SERVER_TEST_MIGRATIONS_DIR);
    migrations.Run();

    m_session = std::make_shared<smtp::SmtpSession>(m_connectionId, m_context, m_handler);
  }

  void TearDown() override
  {
    RemoveDatabase();
  }

  void RemoveDatabase() const
  {
    std::remove(m_databasePath.c_str());
    std::remove((m_databasePath + "-wal").c_str());
    std::remove((m_databasePath + "-shm").c_str());
  }

  std::string PrepareDatabasePath(const std::string& path)
  {
    std::remove(path.c_str());
    std::remove((path + "-wal").c_str());
    std::remove((path + "-shm").c_str());
    return path;
  }


  const std::string m_databasePath{"smtp-session-storage-test.db"};
  const smtp::ConnectionId m_connectionId{1};

  Storage::Database m_database{PrepareDatabasePath(m_databasePath)};
  Storage::MailMessageRepository m_mailMessages{m_database};
  Storage::MessageRecipientRepository m_messageRecipients{m_database};

  std::mutex m_storageMutex;
  smtp::ServerConfig m_config;
  TestSocketsManager m_socketsManager;
  smtp::AuthService m_authService;
  TestLogger m_logger;

  smtp::SmtpSessionContext m_context{
      m_config,
      m_socketsManager,
      m_authService,
      m_database,
      m_mailMessages,
      m_messageRecipients,
      m_storageMutex,
      m_logger};

  smtp::SmtpSessionHandler m_handler;

  std::shared_ptr<smtp::SmtpSession> m_session;
};

TEST_F(SessionStorageTest, StoresAcceptedMessageAndQueuesRecipients)
{
  Send("HELO client.test");
  Send( "MAIL FROM:<sender@example.com>");
  Send("RCPT TO:<first@example.com>");
  Send("RCPT TO:<second@example.com>");
  Send("DATA");
  Send("Message body");
  Send(".");

  const std::vector<Storage::MailMessageRecord> queued_messages =
    m_mailMessages.FindByStatus(Storage::MailMessageStatus::Queued, 10);
  ASSERT_EQ(queued_messages.size(), 1);
  EXPECT_EQ(queued_messages.front().sender_email, "sender@example.com");
  EXPECT_EQ(queued_messages.front().body, "Message body\r\n");

  const std::vector<Storage::MessageRecipientRecord> recipients =
    m_messageRecipients.FindByMessageId(queued_messages.front().id);
  ASSERT_EQ(recipients.size(), 2);
  EXPECT_EQ(recipients.front().delivery_status, Storage::DeliveryStatus::Queued);
  EXPECT_EQ(recipients.back().delivery_status, Storage::DeliveryStatus::Queued);
}

TEST_F(SessionStorageTest, RejectRCPTBeforeMAIL)
{
  Send("HELO client.test");
  Send("RCPT TO:<first@example.com>");

  EXPECT_EQ(GetLastErrorMessage(), "503");
  EXPECT_EQ(m_state.phase, smtp::SmtpSessionPhase::ReadyForMail);
}

TEST_F(SessionStorageTest, RejectDATABeforeRCPT)
{
  Send("HELO client.test");
  Send( "MAIL FROM:<sender@example.com>");
  Send("DATA");

  EXPECT_EQ(GetLastErrorMessage(), "503");
  EXPECT_EQ(m_state.phase, smtp::SmtpSessionPhase::MailTransaction);
}

TEST_F(SessionStorageTest, RejectMessageBiggerThanLimit)
{
  m_config.maxMessageSizeBytes = 1;

  Send("HELO client.test");
  Send( "MAIL FROM:<sender@example.com>");
  Send("RCPT TO:<first@example.com>");
  Send("RCPT TO:<second@example.com>");
  Send("DATA");
  Send("Message body");
  Send(".");

  auto queued = m_mailMessages.FindByStatus(Storage::MailMessageStatus::Queued, 10);
  EXPECT_TRUE(queued.empty());
}

TEST_F(SessionStorageTest, CheckDataResetingAfterTransaction)
{
  m_config.maxMessageSizeBytes = 1;

  Send("HELO client.test");
  Send( "MAIL FROM:<sender@example.com>");
  Send("RCPT TO:<first@example.com>");
  Send("RCPT TO:<second@example.com>");
  Send("DATA");
  Send("Message body");
  Send(".");

  EXPECT_TRUE(m_state.recipients.empty());
  EXPECT_EQ(m_state.sender, "");

  Send( "MAIL FROM:<notsender@example.com>");
  EXPECT_TRUE(m_state.recipients.empty());
  EXPECT_EQ(m_state.sender, "notsender@example.com");
}

TEST_F(SessionStorageTest, CheckCloseStatementWithQUIT)
{
  Send("HELO client.test");
  Send("QUIT");

  EXPECT_EQ(m_state.phase, smtp::SmtpSessionPhase::Closing);
}

TEST_F(SessionStorageTest, CheckCloseStatementWithDirectEvent)
{
  Send("HELO client.test");
  m_handler.HandleEvent({smtp::SmtpEventType::Disconnected, m_connectionId, ""}, m_state, m_context);

  EXPECT_EQ(m_state.phase, smtp::SmtpSessionPhase::Closed);
}

TEST_F(SessionStorageTest, AuthPlainWithValidCredentials)
{
  m_config.allowPlainAuthenticationWithoutTls = true;
  m_authService.AddUser("user", "secret");

  Send("HELO client.test");
  Send("AUTH PLAIN AHVzZXIAc2VjcmV0");

  EXPECT_EQ(GetLastErrorMessage(), "235");
  EXPECT_TRUE(m_state.authenticated);
  EXPECT_EQ(m_state.authenticatedIdentity, "user");
}

TEST_F(SessionStorageTest, AuthLoginWithMultipleSteps)
{
  m_config.allowPlainAuthenticationWithoutTls = true;
  m_authService.AddUser("user", "secret");

  Send("HELO client.test");
  Send("AUTH LOGIN");

  Send("dXNlcg==");
  EXPECT_EQ(GetLastErrorMessage(), "334");
  EXPECT_EQ(m_state.authStage, smtp::SmtpAuthStage::LoginPassword);
  EXPECT_EQ(m_state.pendingAuthUsername, "user");
  EXPECT_FALSE(m_state.authenticated);

  Send("c2VjcmV0");
  EXPECT_EQ(GetLastErrorMessage(), "235");
  EXPECT_EQ(m_state.authStage, smtp::SmtpAuthStage::None);
  EXPECT_TRUE(m_state.authenticated);
  EXPECT_EQ(m_state.authenticatedIdentity, "user");
}

TEST_F(SessionStorageTest, AuthPlainWithInvalidCredentials)
{
  m_config.allowPlainAuthenticationWithoutTls = true;
  m_authService.AddUser("user", "secret");

  Send("HELO client.test");
  Send("AUTH PLAIN AHVzZXIAc2asdgmV0");

  EXPECT_EQ(GetLastErrorMessage(), "535");
  EXPECT_FALSE(m_state.authenticated);
  EXPECT_EQ(m_state.authenticatedIdentity, "");
}

TEST_F(SessionStorageTest, RejectWhenAuthRequiredAndNoAuth)
{
  m_config.requireAuthentication = true;

  Send("HELO client.test");
  Send( "MAIL FROM:<notsender@example.com>");

  EXPECT_EQ(GetLastErrorMessage(), "530");
}

TEST_F(SessionStorageTest, StartTlsRequestedWhenTlsEnabled)
{
  m_config.tls.enabled = true;

  Send("STARTTLS");

  EXPECT_TRUE(m_state.tlsHandshakePending);
}

TEST_F(SessionStorageTest, StartTlsRequestedWhenTlsDisabled)
{
  m_config.tls.enabled = false;

  Send("STARTTLS");

  EXPECT_EQ(GetLastErrorMessage(), "454");
}

TEST_F(SessionStorageTest, SuccessfulTlsEventHandle)
{
  m_config.tls.enabled = true;

  Send("STARTTLS");
  m_handler.HandleEvent({smtp::SmtpEventType::TlsSucceeded, m_connectionId, ""}, m_state, m_context);
  EXPECT_TRUE(m_state.tlsActive);
  EXPECT_FALSE(m_state.tlsHandshakePending);
}

TEST_F(SessionStorageTest, FailedTlsEventHandle)
{
  m_config.tls.enabled = true;

  Send("STARTTLS");
  m_handler.HandleEvent({smtp::SmtpEventType::TlsFailed, m_connectionId, ""}, m_state, m_context);
  EXPECT_FALSE(m_state.tlsActive);
  EXPECT_FALSE(m_state.tlsHandshakePending);
}

TEST_F(SessionStorageTest, RejectPlainAuthWithoutTlsWhenItIsFalse)
{
  m_config.allowPlainAuthenticationWithoutTls = false;
  m_authService.AddUser("user", "secret");

  Send("HELO client.test");
  Send("AUTH PLAIN AHVzZXIAc2VjcmV0");

  EXPECT_EQ(GetLastErrorMessage(), "538");
  EXPECT_FALSE(m_state.authenticated);
  EXPECT_NE(m_state.authenticatedIdentity, "user");
}

TEST_F(SmtpSessionQueueTest, TryExtractTaskWithEmptyQueue)
{
  std::function<void()> task;
  EXPECT_FALSE(m_session->TryExtractNextTask(task));
}

TEST_F(SmtpSessionQueueTest, TryExtractTaskWithNotEmptyQueue)
{
  m_session->PushEvent({smtp::SmtpEventType::Connected, 1, ""});
  std::function<void()> task;
  EXPECT_TRUE(m_session->TryExtractNextTask(task));
  ASSERT_TRUE(task);
}

TEST_F(SmtpSessionQueueTest, TryExtractTasksAtTheSameTime)
{
  m_session->PushEvent({smtp::SmtpEventType::Connected, 1, ""});
  m_session->PushEvent({smtp::SmtpEventType::Connected, 1, ""});

  std::function<void()> task;
  std::function<void()> task1;
  ASSERT_TRUE(m_session->TryExtractNextTask(task));
  EXPECT_FALSE(m_session->TryExtractNextTask(task1));

  task();

  EXPECT_TRUE(m_session->TryExtractNextTask(task1));
}

TEST_F(SmtpSessionQueueTest, IsReadyForRemovalFalseTest)
{
  EXPECT_FALSE(m_session->IsReadyForRemoval());
}

TEST_F(SmtpSessionQueueTest, IsReadyForRemovalAfterDisconnect)
{
  m_session->PushEvent({smtp::SmtpEventType::Disconnected, 1, ""});
  std::function<void()> task;
  m_session->TryExtractNextTask(task);
  task();
  EXPECT_TRUE(m_session->IsReadyForRemoval());
}

TEST_F(SmtpSessionQueueTest, CheckTHeConnectionIdGetter)
{
  EXPECT_EQ(m_session->Connection(), 1);
}
} // namespace
