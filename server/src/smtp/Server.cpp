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
    if (isRunning_) {
        return;
    }

    dependencies_.socketsManager.Start(config_);
    isRunning_ = true;
}

void SmtpServer::Run()
{
    Start();

    while (isRunning_) {
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
    if (!isRunning_) {
        return;
    }

    isRunning_ = false;
    dependencies_.socketsManager.Stop();
    sessions_.clear();
}

bool SmtpServer::IsRunning() const
{
    return isRunning_;
}

std::size_t SmtpServer::ConnectionIdHash::operator()(ConnectionId connectionId) const noexcept
{
    return std::hash<std::uint64_t>{}(connectionId.value);
}

void SmtpServer::RouteEvent(SmtpEvent event)
{
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
