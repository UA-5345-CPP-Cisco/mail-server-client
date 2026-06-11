#include "smtp/Server.hpp"

#include <algorithm>
#include <cctype>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace smtp {

namespace {

const char* ToString(SmtpEventType type)
{
    switch (type) {
    case SmtpEventType::Connected:
        return "Connected";
    case SmtpEventType::MessageReceived:
        return "MessageReceived";
    case SmtpEventType::TlsSucceeded:
        return "TlsSucceeded";
    case SmtpEventType::TlsFailed:
        return "TlsFailed";
    case SmtpEventType::Disconnected:
        return "Disconnected";
    }

    return "Unknown";
}

std::string FormatPayload(std::string_view payload)
{
    constexpr std::size_t maxPayloadLength = 160;

    std::string result;
    result.reserve(std::min(payload.size(), maxPayloadLength) + 8);

    std::size_t charactersWritten = 0;
    for (char character : payload) {
        if (charactersWritten >= maxPayloadLength) {
            result += "...";
            break;
        }

        switch (character) {
        case '\r':
            result += "\\r";
            break;
        case '\n':
            result += "\\n";
            break;
        case '\t':
            result += "\\t";
            break;
        case '"':
            result += "\\\"";
            break;
        case '\\':
            result += "\\\\";
            break;
        default:
            result.push_back(character);
            break;
        }

        ++charactersWritten;
    }

    return result;
}

bool StartsWithCaseInsensitive(std::string_view value, std::string_view prefix)
{
    if (value.size() < prefix.size()) {
        return false;
    }

    for (std::size_t index = 0; index < prefix.size(); ++index) {
        if (std::toupper(static_cast<unsigned char>(value[index])) !=
            std::toupper(static_cast<unsigned char>(prefix[index]))) {
            return false;
        }
    }

    return true;
}

std::string FormatEventForLog(const SmtpEvent& event)
{
    std::ostringstream stream;
    stream << "SMTP event " << ToString(event.type)
           << " connection=" << event.connectionId;

    if (!event.payload.empty()) {
        if (StartsWithCaseInsensitive(event.payload, "AUTH ")) {
            stream << " payload=\"AUTH <redacted>\"";
        } else {
            stream << " payload=\"" << FormatPayload(event.payload) << '"';
        }
    }

    return stream.str();
}

}

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

    dependencies_.logger.Log(
        LogLevel::Info,
        "SMTP server listening on " + config_.host + ":" + std::to_string(config_.port));

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

    dependencies_.logger.Log(LogLevel::Info, "SMTP server stopping");
    dependencies_.socketsManager.Stop();
    sessions_.clear();
}

bool SmtpServer::IsRunning() const
{
    return isRunning_.load();
}

void SmtpServer::RouteEvent(SmtpEvent event)
{
    dependencies_.logger.Log(LogLevel::Info, FormatEventForLog(event));

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
        config_,
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
