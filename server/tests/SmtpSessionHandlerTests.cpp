#include "TestDoubles.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace smtp::test {

namespace {

void ExpectLastReply(const HandlerHarness& harness, const std::string& expected)
{
    EXPECT_EQ(harness.sockets.LastSentText(HandlerHarness::connectionId), expected);
}

}

TEST(SmtpSessionHandlerTests, ConnectedSendsGreeting)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::Connected);

    ASSERT_EQ(harness.sockets.sentMessages.size(), 1U);
    EXPECT_EQ(harness.sockets.sentMessages[0].text, "220 mx.test ESMTP ready\r\n");
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::WaitingForGreeting);
}

TEST(SmtpSessionHandlerTests, EhloAdvertisesSupportedCapabilities)
{
    HandlerHarness harness;
    harness.config.tls.enabled = true;
    harness.config.allowPlainAuthenticationWithoutTls = true;
    harness.config.maxMessageSizeBytes = 4096;

    harness.Deliver(SmtpEventType::MessageReceived, "EHLO client.example\r\n");

    const std::vector<std::string> expected{
        "250-mx.test\r\n",
        "250-STARTTLS\r\n",
        "250-AUTH PLAIN LOGIN\r\n",
        "250 SIZE 4096\r\n"
    };

    EXPECT_EQ(harness.sockets.SentTexts(HandlerHarness::connectionId), expected);
    EXPECT_EQ(harness.state.clientName, "client.example");
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::ReadyForMail);
}

TEST(SmtpSessionHandlerTests, HeloSendsSimpleGreetingReply)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "HELO client.example");

    ExpectLastReply(harness, "250 mx.test\r\n");
    EXPECT_EQ(harness.state.clientName, "client.example");
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::ReadyForMail);
}

TEST(SmtpSessionHandlerTests, StartTlsBeginsHandshakeAndResetsConversation)
{
    HandlerHarness harness;
    harness.config.tls.enabled = true;
    harness.state.phase = SmtpSessionPhase::MailTransaction;
    harness.state.clientName = "client.example";
    harness.state.authenticated = true;
    harness.state.authenticatedIdentity = "alice";
    harness.state.sender = "alice@example.com";
    harness.state.recipients.push_back("bob@example.com");

    harness.Deliver(SmtpEventType::MessageReceived, "STARTTLS");

    ExpectLastReply(harness, "220 Ready to start TLS\r\n");
    ASSERT_EQ(harness.sockets.tlsStarts.size(), 1U);
    EXPECT_EQ(harness.sockets.tlsStarts[0], HandlerHarness::connectionId);
    EXPECT_TRUE(harness.state.tlsHandshakePending);
    EXPECT_FALSE(harness.state.tlsActive);
    EXPECT_FALSE(harness.state.authenticated);
    EXPECT_TRUE(harness.state.clientName.empty());
    EXPECT_TRUE(harness.state.sender.empty());
    EXPECT_TRUE(harness.state.recipients.empty());
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::WaitingForGreeting);

    harness.Deliver(SmtpEventType::TlsSucceeded);

    EXPECT_TRUE(harness.state.tlsActive);
    EXPECT_FALSE(harness.state.tlsHandshakePending);
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::WaitingForGreeting);
}

TEST(SmtpSessionHandlerTests, StartTlsDisabledReturnsTemporaryFailure)
{
    HandlerHarness harness;
    harness.config.tls.enabled = false;

    harness.Deliver(SmtpEventType::MessageReceived, "STARTTLS");

    ExpectLastReply(harness, "454 TLS not available\r\n");
    EXPECT_TRUE(harness.sockets.tlsStarts.empty());
}

TEST(SmtpSessionHandlerTests, TlsFailureClosesConnection)
{
    HandlerHarness harness;
    harness.state.tlsHandshakePending = true;

    harness.Deliver(SmtpEventType::TlsFailed);

    EXPECT_FALSE(harness.state.tlsHandshakePending);
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::Closed);
    ASSERT_EQ(harness.sockets.closedConnections.size(), 1U);
    EXPECT_EQ(harness.sockets.closedConnections[0], HandlerHarness::connectionId);
}

TEST(SmtpSessionHandlerTests, AuthPlainRequiresTlsUnlessExplicitlyAllowed)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "AUTH PLAIN AGFsaWNlAHNlY3JldA==");

    ExpectLastReply(harness, "538 Encryption required for requested authentication mechanism\r\n");
    EXPECT_TRUE(harness.auth.requests.empty());
    EXPECT_FALSE(harness.state.authenticated);
}

TEST(SmtpSessionHandlerTests, AuthPlainAcceptsValidInitialResponse)
{
    HandlerHarness harness;
    harness.config.allowPlainAuthenticationWithoutTls = true;

    harness.Deliver(SmtpEventType::MessageReceived, "AUTH PLAIN AGFsaWNlAHNlY3JldA==");

    ASSERT_EQ(harness.auth.requests.size(), 1U);
    EXPECT_EQ(harness.auth.requests[0].mechanism, "PLAIN");
    EXPECT_EQ(harness.auth.requests[0].username, "alice");
    EXPECT_EQ(harness.auth.requests[0].secret, "secret");
    EXPECT_TRUE(harness.state.authenticated);
    EXPECT_EQ(harness.state.authenticatedIdentity, "alice");
    ExpectLastReply(harness, "235 Authentication successful\r\n");
}

TEST(SmtpSessionHandlerTests, AuthLoginSequenceAuthenticates)
{
    HandlerHarness harness;
    harness.config.allowPlainAuthenticationWithoutTls = true;

    harness.Deliver(SmtpEventType::MessageReceived, "AUTH LOGIN");
    ExpectLastReply(harness, "334 VXNlcm5hbWU6\r\n");
    EXPECT_EQ(harness.state.authStage, SmtpAuthStage::LoginUsername);

    harness.Deliver(SmtpEventType::MessageReceived, "YWxpY2U=");
    ExpectLastReply(harness, "334 UGFzc3dvcmQ6\r\n");
    EXPECT_EQ(harness.state.authStage, SmtpAuthStage::LoginPassword);

    harness.Deliver(SmtpEventType::MessageReceived, "c2VjcmV0");

    ASSERT_EQ(harness.auth.requests.size(), 1U);
    EXPECT_EQ(harness.auth.requests[0].mechanism, "LOGIN");
    EXPECT_EQ(harness.auth.requests[0].username, "alice");
    EXPECT_EQ(harness.auth.requests[0].secret, "secret");
    EXPECT_TRUE(harness.state.authenticated);
    EXPECT_EQ(harness.state.authStage, SmtpAuthStage::None);
    ExpectLastReply(harness, "235 Authentication successful\r\n");
}

TEST(SmtpSessionHandlerTests, AuthFailureKeepsSessionUnauthenticated)
{
    HandlerHarness harness;
    harness.config.allowPlainAuthenticationWithoutTls = true;
    harness.auth.accepted = false;

    harness.Deliver(SmtpEventType::MessageReceived, "AUTH PLAIN AGFsaWNlAHdyb25n");

    EXPECT_FALSE(harness.state.authenticated);
    ExpectLastReply(harness, "535 Authentication failed\r\n");
}

TEST(SmtpSessionHandlerTests, MailboxCommandsRequireAuthentication)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "MAILCOUNT");
    ExpectLastReply(harness, "530 Authentication required\r\n");

    harness.Deliver(SmtpEventType::MessageReceived, "GETMAILS 1 10");
    ExpectLastReply(harness, "530 Authentication required\r\n");
}

TEST(SmtpSessionHandlerTests, MailboxCommandsReturnCountAndMessageRange)
{
    HandlerHarness harness;
    harness.state.authenticated = true;
    harness.state.authenticatedIdentity = "bob@example.com";
    harness.storage.mailboxMessages["bob@example.com"] = {
        StoredMailMessage{1, "10", MailMessage{"alice@example.com", {"bob@example.com"}, "raw one"}},
        StoredMailMessage{2, "11", MailMessage{"carol@example.com", {"bob@example.com"}, "raw two"}}
    };

    harness.Deliver(SmtpEventType::MessageReceived, "MAILCOUNT");
    ExpectLastReply(harness, "250 2\r\n");

    harness.Deliver(SmtpEventType::MessageReceived, "GETMAILS 1 2");

    const std::vector<std::string> sent = harness.sockets.SentTexts(HandlerHarness::connectionId);
    ASSERT_GE(sent.size(), 4U);
    EXPECT_EQ(sent[sent.size() - 3], "250-MAIL 1 10 cmF3IG9uZQ==\r\n");
    EXPECT_EQ(sent[sent.size() - 2], "250-MAIL 2 11 cmF3IHR3bw==\r\n");
    EXPECT_EQ(sent[sent.size() - 1], "250 OK\r\n");
}

TEST(SmtpSessionHandlerTests, MailRequiresAuthenticationWhenConfigured)
{
    HandlerHarness harness;
    harness.config.requireAuthentication = true;

    harness.Deliver(SmtpEventType::MessageReceived, "MAIL FROM:<alice@example.com>");

    ExpectLastReply(harness, "530 Authentication required\r\n");
    EXPECT_TRUE(harness.state.sender.empty());

    harness.state.authenticated = true;
    harness.Deliver(SmtpEventType::MessageReceived, "MAIL FROM:<alice@example.com>");

    ExpectLastReply(harness, "250 Sender OK\r\n");
    EXPECT_EQ(harness.state.sender, "alice@example.com");
}

TEST(SmtpSessionHandlerTests, AcceptsMessageStoresCachesQueuesAndResetsTransaction)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "MAIL FROM:<alice@example.com>");
    harness.Deliver(SmtpEventType::MessageReceived, "RCPT TO:<bob@example.com>");
    harness.Deliver(SmtpEventType::MessageReceived, "DATA");
    harness.Deliver(SmtpEventType::MessageReceived, "Subject: Hi");
    harness.Deliver(SmtpEventType::MessageReceived, "..dot-stuffed");
    harness.Deliver(SmtpEventType::MessageReceived, ".");

    ASSERT_EQ(harness.lookup.requests.size(), 1U);
    EXPECT_EQ(harness.lookup.requests[0].key, "EXAMPLE.COM");

    ASSERT_EQ(harness.storage.savedMessages.size(), 1U);
    EXPECT_EQ(harness.storage.savedMessages[0].sender, "alice@example.com");
    ASSERT_EQ(harness.storage.savedMessages[0].recipients.size(), 1U);
    EXPECT_EQ(harness.storage.savedMessages[0].recipients[0], "bob@example.com");
    EXPECT_EQ(harness.storage.savedMessages[0].rawContent, "Subject: Hi\r\n.dot-stuffed\r\n");

    ASSERT_EQ(harness.delivery.queuedMessages.size(), 1U);
    EXPECT_EQ(harness.delivery.queuedMessages[0].rawContent, "Subject: Hi\r\n.dot-stuffed\r\n");

    ASSERT_TRUE(harness.cache.values.contains("message:42"));
    EXPECT_EQ(harness.cache.values["message:42"], "Subject: Hi\r\n.dot-stuffed\r\n");
    ExpectLastReply(harness, "250 Message accepted as 42\r\n");
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::ReadyForMail);
    EXPECT_TRUE(harness.state.sender.empty());
    EXPECT_TRUE(harness.state.recipients.empty());
    EXPECT_TRUE(harness.state.messageBuffer.empty());
}

TEST(SmtpSessionHandlerTests, RejectsRecipientWhenLookupFails)
{
    HandlerHarness harness;
    harness.lookup.found = false;

    harness.Deliver(SmtpEventType::MessageReceived, "MAIL FROM:<alice@example.com>");
    harness.Deliver(SmtpEventType::MessageReceived, "RCPT TO:<bob@example.com>");

    ExpectLastReply(harness, "550 Recipient rejected\r\n");
    EXPECT_TRUE(harness.state.recipients.empty());
}

TEST(SmtpSessionHandlerTests, RejectsInvalidCommandSequence)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "RCPT TO:<bob@example.com>");
    ExpectLastReply(harness, "503 Need MAIL FROM before RCPT TO\r\n");

    harness.Deliver(SmtpEventType::MessageReceived, "DATA");
    ExpectLastReply(harness, "503 Need MAIL FROM and RCPT TO before DATA\r\n");
}

TEST(SmtpSessionHandlerTests, RejectsOversizedDataAndResetsTransaction)
{
    HandlerHarness harness;
    harness.config.maxMessageSizeBytes = 8;

    harness.Deliver(SmtpEventType::MessageReceived, "MAIL FROM:<alice@example.com>");
    harness.Deliver(SmtpEventType::MessageReceived, "RCPT TO:<bob@example.com>");
    harness.Deliver(SmtpEventType::MessageReceived, "DATA");
    harness.Deliver(SmtpEventType::MessageReceived, "123456789");

    ExpectLastReply(harness, "552 Message size exceeds fixed maximum message size\r\n");
    EXPECT_TRUE(harness.storage.savedMessages.empty());
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::ReadyForMail);
    EXPECT_TRUE(harness.state.sender.empty());
}

TEST(SmtpSessionHandlerTests, RsetClearsMailTransaction)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "MAIL FROM:<alice@example.com>");
    harness.Deliver(SmtpEventType::MessageReceived, "RCPT TO:<bob@example.com>");
    harness.Deliver(SmtpEventType::MessageReceived, "RSET");

    ExpectLastReply(harness, "250 Reset OK\r\n");
    EXPECT_TRUE(harness.state.sender.empty());
    EXPECT_TRUE(harness.state.recipients.empty());
    EXPECT_TRUE(harness.state.messageBuffer.empty());
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::ReadyForMail);
}

TEST(SmtpSessionHandlerTests, ClassicUtilityCommandsReturnExpectedReplies)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "VRFY alice@example.com");
    ExpectLastReply(harness, "252 Cannot VRFY user, but will accept message\r\n");

    harness.Deliver(SmtpEventType::MessageReceived, "EXPN list@example.com");
    ExpectLastReply(harness, "502 EXPN not implemented\r\n");

    harness.Deliver(SmtpEventType::MessageReceived, "HELP");
    ExpectLastReply(harness,
                    "214 Commands: HELO EHLO MAIL RCPT DATA RSET VRFY EXPN HELP NOOP QUIT STARTTLS AUTH MAILCOUNT GETMAILS\r\n");

    harness.Deliver(SmtpEventType::MessageReceived, "NOOP");
    ExpectLastReply(harness, "250 OK\r\n");
}

TEST(SmtpSessionHandlerTests, QuitRepliesAndClosesConnection)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "QUIT");

    ExpectLastReply(harness, "221 mx.test closing connection\r\n");
    ASSERT_EQ(harness.sockets.closedConnections.size(), 1U);
    EXPECT_EQ(harness.sockets.closedConnections[0], HandlerHarness::connectionId);
    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::Closing);
}

TEST(SmtpSessionHandlerTests, UnknownCommandReturnsSyntaxError)
{
    HandlerHarness harness;

    harness.Deliver(SmtpEventType::MessageReceived, "WAT");

    ExpectLastReply(harness, "500 Command unrecognized\r\n");
}

TEST(SmtpSessionHandlerTests, DisconnectedClosesSessionStateWithoutSending)
{
    HandlerHarness harness;
    harness.state.sender = "alice@example.com";
    harness.state.recipients.push_back("bob@example.com");
    harness.state.messageBuffer = "body";

    harness.Deliver(SmtpEventType::Disconnected);

    EXPECT_EQ(harness.state.phase, SmtpSessionPhase::Closed);
    EXPECT_TRUE(harness.state.sender.empty());
    EXPECT_TRUE(harness.state.recipients.empty());
    EXPECT_TRUE(harness.state.messageBuffer.empty());
    EXPECT_TRUE(harness.sockets.sentMessages.empty());
}

}
