#include "TestDoubles.hpp"

#include "smtp/Server.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <string_view>
#include <utility>
#include <vector>

namespace smtp::test {

namespace {

bool HasLogEntry(const RecordingLogger& logger, LogLevel level, std::string_view fragment)
{
    return std::any_of(logger.entries.begin(), logger.entries.end(), [level, fragment](const auto& entry) {
        return entry.level == level && entry.message.find(fragment) != std::string::npos;
    });
}

}

struct ServerHarness {
    static ServerConfig MakeConfig()
    {
        ServerConfig result;
        result.serverName = "mx.test";
        return result;
    }

    explicit ServerHarness(ServerConfig configValue = MakeConfig())
        : config(std::move(configValue)),
          dependencies{
              sockets,
              threadPool,
              handler,
              auth,
              storage,
              cache,
              delivery,
              lookup,
              logger
          },
          server(config, dependencies)
    {
    }

    ServerConfig config;
    FakeSocketsManager sockets;
    ImmediateThreadPool threadPool;
    SmtpSessionHandler handler;
    FakeAuthService auth;
    FakeMailStorage storage;
    FakeCacheService cache;
    FakeDeliveryService delivery;
    FakeLookupService lookup;
    RecordingLogger logger;
    SmtpServerDependencies dependencies;
    SmtpServer server;
};

TEST(ServerTests, RunOnceRoutesNewConnectionAndSchedulesOneSessionTask)
{
    ServerHarness harness;
    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::Connected, 1, ""});

    harness.server.RunOnce();

    EXPECT_EQ(harness.threadPool.enqueueCount, 1);
    ASSERT_EQ(harness.sockets.sentMessages.size(), 1U);
    EXPECT_EQ(harness.sockets.sentMessages[0].connectionId, 1U);
    EXPECT_EQ(harness.sockets.sentMessages[0].text, "220 mx.test ESMTP ready\r\n");
    EXPECT_EQ(harness.sockets.continuedConnections, std::vector<ConnectionId>{1});
    EXPECT_TRUE(HasLogEntry(harness.logger, LogLevel::Info, "SMTP event Connected connection=1"));
}

TEST(ServerTests, UnknownDisconnectedEventIsIgnored)
{
    ServerHarness harness;
    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::Disconnected, 1, ""});

    harness.server.RunOnce();

    EXPECT_EQ(harness.threadPool.enqueueCount, 0);
    EXPECT_TRUE(harness.sockets.sentMessages.empty());
    EXPECT_TRUE(harness.sockets.closedConnections.empty());
    EXPECT_TRUE(HasLogEntry(harness.logger, LogLevel::Info, "SMTP event Disconnected connection=1"));
}

TEST(ServerTests, RunOnceLogsMessagePayloads)
{
    ServerHarness harness;
    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::Connected, 1, ""});
    harness.server.RunOnce();

    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::MessageReceived, 1, "EHLO client\r\n"});
    harness.server.RunOnce();

    EXPECT_TRUE(HasLogEntry(harness.logger,
                            LogLevel::Info,
                            "SMTP event MessageReceived connection=1 payload=\"EHLO client\\r\\n\""));
}

TEST(ServerTests, ConnectionLimitClosesExtraNewConnections)
{
    ServerConfig config = ServerHarness::MakeConfig();
    config.maxConnections = 1;
    ServerHarness harness(config);
    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::Connected, 1, ""});
    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::Connected, 2, ""});

    harness.server.RunOnce();

    EXPECT_EQ(harness.threadPool.enqueueCount, 1);
    ASSERT_EQ(harness.sockets.closedConnections.size(), 1U);
    EXPECT_EQ(harness.sockets.closedConnections[0], 2U);
    EXPECT_TRUE(HasLogEntry(harness.logger, LogLevel::Warning, "SMTP connection limit reached"));
}

TEST(ServerTests, ClosedSessionsAreRemovedAfterDisconnectTaskCompletes)
{
    ServerHarness harness;
    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::Connected, 1, ""});
    harness.server.RunOnce();

    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::Disconnected, 1, ""});
    harness.server.RunOnce();

    EXPECT_EQ(harness.threadPool.enqueueCount, 2);

    harness.sockets.pendingEvents.push_back(SmtpEvent{SmtpEventType::Disconnected, 1, ""});
    harness.server.RunOnce();

    EXPECT_EQ(harness.threadPool.enqueueCount, 2);
}

TEST(ServerTests, StopBeforeStartDoesNotStopSockets)
{
    ServerHarness harness;

    harness.server.Stop();

    EXPECT_FALSE(harness.server.IsRunning());
    EXPECT_FALSE(harness.sockets.stopped);
}

}
