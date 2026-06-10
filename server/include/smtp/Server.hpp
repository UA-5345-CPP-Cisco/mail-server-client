#pragma once

#include "smtp/Event.hpp"
#include "smtp/Logger.hpp"
#include "smtp/ServerConfig.hpp"
#include "smtp/Services.hpp"
#include "smtp/Session.hpp"
#include "smtp/SocketsManager.hpp"
#include "smtp/ThreadPool.hpp"

#include <atomic>
#include <cstddef>
#include <memory>
#include <unordered_map>

namespace smtp {

// References to infrastructure and service components used by the server.
// SmtpServer coordinates these objects but does not own them.
struct SmtpServerDependencies {
    ISocketsManager& socketsManager;
    IThreadPool& threadPool;
    SmtpSessionHandler& sessionHandler;
    IAuthService& authService;
    IMailStorage& mailStorage;
    ICacheService& cacheService;
    IDeliveryService& deliveryService;
    ILookupService& lookupService;
    ILogger& logger;
};

// Server-level orchestrator.
//
// Responsibilities:
// - start and stop the sockets manager;
// - run the listening/scheduling loop;
// - route completed socket events to the correct SmtpSession;
// - create one SmtpSession per connection id;
// - ask sessions for safe-to-run tasks;
// - enqueue those tasks into the thread pool;
// - remove closed sessions after their queued work is complete.
//
// It should not parse SMTP commands, touch raw sockets directly, or implement
// storage/auth/delivery/lookup behavior.
class SmtpServer {
public:
    SmtpServer(ServerConfig config, SmtpServerDependencies dependencies);

    // Keep running server iterations until Stop() is requested.
    void Start();

    // Execute one server iteration: poll events, route them, schedule tasks,
    // and clean finished sessions.
    void RunOnce();

    // Stop accepting work, stop sockets, and release session state.
    void Stop();

    bool IsRunning() const;

private:
    void RouteEvent(SmtpEvent event);
    void ScheduleReadySessions();
    void RemoveClosedSessions();
    std::shared_ptr<SmtpSession> GetOrCreateSession(ConnectionId connectionId);

    ServerConfig config_;
    SmtpServerDependencies dependencies_;
    std::atomic_bool isRunning_{false};
    std::unordered_map<ConnectionId, std::shared_ptr<SmtpSession>> sessions_;
};

}
