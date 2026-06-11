#include "TestDoubles.hpp"

#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <vector>

namespace smtp::test {

namespace {

void RunNextTask(const std::shared_ptr<SmtpSession>& session)
{
    std::function<void()> task;
    ASSERT_TRUE(session->TryExtractNextTask(task));
    task();
}

}

TEST(SessionTests, ExtractsOnlyOneTaskUntilTheCurrentTaskFinishes)
{
    HandlerHarness harness;
    auto session = std::make_shared<SmtpSession>(
        HandlerHarness::connectionId,
        harness.context,
        harness.handler);

    session->PushEvent(SmtpEvent{SmtpEventType::Connected, HandlerHarness::connectionId, ""});
    session->PushEvent(SmtpEvent{SmtpEventType::MessageReceived, HandlerHarness::connectionId, "NOOP"});

    std::function<void()> firstTask;
    ASSERT_TRUE(session->TryExtractNextTask(firstTask));

    std::function<void()> secondTask;
    EXPECT_FALSE(session->TryExtractNextTask(secondTask));

    firstTask();

    ASSERT_TRUE(session->TryExtractNextTask(secondTask));
    secondTask();

    const std::vector<std::string> sent = harness.sockets.SentTexts(HandlerHarness::connectionId);
    ASSERT_EQ(sent.size(), 2U);
    EXPECT_EQ(sent[0], "220 mx.test ESMTP ready\r\n");
    EXPECT_EQ(sent[1], "250 OK\r\n");

    const std::vector<ConnectionId> expectedContinues{
        HandlerHarness::connectionId,
        HandlerHarness::connectionId
    };
    EXPECT_EQ(harness.sockets.continuedConnections, expectedContinues);
}

TEST(SessionTests, DisconnectedEventMarksSessionReadyForRemoval)
{
    HandlerHarness harness;
    auto session = std::make_shared<SmtpSession>(
        HandlerHarness::connectionId,
        harness.context,
        harness.handler);

    session->PushEvent(SmtpEvent{SmtpEventType::Disconnected, HandlerHarness::connectionId, ""});
    RunNextTask(session);

    EXPECT_TRUE(session->IsReadyForRemoval());
    EXPECT_TRUE(harness.sockets.continuedConnections.empty());
}

TEST(SessionTests, HandlerExceptionClosesConnectionLogsAndReleasesTheNextTask)
{
    HandlerHarness harness;
    harness.storage.throwOnSave = true;
    auto session = std::make_shared<SmtpSession>(
        HandlerHarness::connectionId,
        harness.context,
        harness.handler);

    session->PushEvent(SmtpEvent{SmtpEventType::MessageReceived, HandlerHarness::connectionId, "MAIL FROM:<alice@example.com>"});
    session->PushEvent(SmtpEvent{SmtpEventType::MessageReceived, HandlerHarness::connectionId, "RCPT TO:<bob@example.com>"});
    session->PushEvent(SmtpEvent{SmtpEventType::MessageReceived, HandlerHarness::connectionId, "DATA"});
    session->PushEvent(SmtpEvent{SmtpEventType::MessageReceived, HandlerHarness::connectionId, "."});
    session->PushEvent(SmtpEvent{SmtpEventType::MessageReceived, HandlerHarness::connectionId, "NOOP"});

    RunNextTask(session);
    RunNextTask(session);
    RunNextTask(session);
    RunNextTask(session);

    ASSERT_EQ(harness.sockets.closedConnections.size(), 1U);
    EXPECT_EQ(harness.sockets.closedConnections[0], HandlerHarness::connectionId);
    ASSERT_EQ(harness.logger.entries.size(), 1U);
    EXPECT_EQ(harness.logger.entries[0].level, LogLevel::Error);
    EXPECT_EQ(harness.logger.entries[0].message, "storage failure");

    std::function<void()> nextTask;
    EXPECT_TRUE(session->TryExtractNextTask(nextTask));
}

}
