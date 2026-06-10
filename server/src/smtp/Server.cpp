#include "smtp/Server.hpp"

#include <functional>
#include <utility>
#include <vector>

namespace smtp {

SmtpServer::SmtpServer(ServerConfig config, SmtpServerDependencies dependencies)
    : config_(std::move(config)),
      dependencies_(dependencies)
{
}

void SmtpServer::Start()
{
    bool expected = false;
    if (!isRunning_.compare_exchange_strong(expected, true)) {
        return;
    }

    try {
        dependencies_.socketsManager.Start(config_);
    } catch (...) {
        isRunning_.store(false);
        throw;
    }

    while (isRunning_.load()) {
        RunOnce();
    }
}


void SmtpServer::RunOnce()
{
    std::vector<SmtpEvent> events = dependencies_.socketsManager.PollEvents();

    for (SmtpEvent& event : events) {
        RouteEvent(std::move(event));
    }

    ScheduleReadySessions();
    RemoveClosedSessions();
}

void SmtpServer::Stop()
{
    bool expected = true;
    if (!isRunning_.compare_exchange_strong(expected, false)) {
        return;
    }

    dependencies_.socketsManager.Stop();
    sessions_.clear();
}

bool SmtpServer::IsRunning() const
{
    return isRunning_.load();
}

void SmtpServer::RouteEvent(SmtpEvent event)
{
    auto existing = sessions_.find(event.connectionId);
    if (existing != sessions_.end()) {
        existing->second->PushEvent(std::move(event));
        return;
    }

    if (event.type == SmtpEventType::Disconnected) {
        return;
    }

    if (sessions_.size() >= config_.maxConnections) {
        dependencies_.logger.Log(LogLevel::Warning, "SMTP connection limit reached");
        dependencies_.socketsManager.Close(event.connectionId);
        return;
    }

    std::shared_ptr<SmtpSession> session = GetOrCreateSession(event.connectionId);
    session->PushEvent(std::move(event));
}

void SmtpServer::ScheduleReadySessions()
{
    for (auto& entry : sessions_) {
        std::function<void()> task;

        if (entry.second->TryExtractNextTask(task)) {
            dependencies_.threadPool.Enqueue(std::move(task));
        }
    }
}

void SmtpServer::RemoveClosedSessions()
{
    for (auto iterator = sessions_.begin(); iterator != sessions_.end();) {
        if (iterator->second->IsReadyForRemoval()) {
            iterator = sessions_.erase(iterator);
        } else {
            ++iterator;
        }
    }
}

std::shared_ptr<SmtpSession> SmtpServer::GetOrCreateSession(ConnectionId connectionId)
{
    auto existing = sessions_.find(connectionId);
    if (existing != sessions_.end()) {
        return existing->second;
    }

    SmtpSessionContext context{
        dependencies_.socketsManager,
        dependencies_.authService,
        dependencies_.mailStorage,
        dependencies_.cacheService,
        dependencies_.deliveryService,
        dependencies_.lookupService,
        dependencies_.logger
    };

    auto session = std::make_shared<SmtpSession>(
        connectionId,
        context,
        dependencies_.sessionHandler);

    sessions_.emplace(connectionId, session);
    return session;
}

}
