#pragma once

#include "smtp/Event.hpp"
#include "smtp/Logger.hpp"
#include "smtp/ServerConfig.hpp"
#include "smtp/Services.hpp"
#include "smtp/Session.hpp"
#include "smtp/SocketsManager.hpp"
#include "smtp/ThreadPool.hpp"

#include <cstddef>
#include <memory>
#include <unordered_map>

namespace smtp {

struct SmtpServerDependencies {
    ISocketsManager& socketsManager;
    IThreadPool& threadPool;
    ISmtpSessionHandler& sessionHandler;
    IAuthService& authService;
    IMailStorage& mailStorage;
    ICacheService& cacheService;
    IDeliveryService& deliveryService;
    ILookupService& lookupService;
    ILogger& logger;
};

class SmtpServer {
public:
    SmtpServer(ServerConfig config, SmtpServerDependencies dependencies);

    void Start();
    void Run();
    void RunOnce();
    void Stop();
    bool IsRunning() const;

private:
    struct ConnectionIdHash {
        std::size_t operator()(ConnectionId connectionId) const noexcept;
    };

    void RouteEvent(SmtpEvent event);
    void ScheduleReadySessions();
    void RemoveClosedSessions();
    std::shared_ptr<SmtpSession> GetOrCreateSession(ConnectionId connectionId);

    ServerConfig config_;
    SmtpServerDependencies dependencies_;
    bool isRunning_{false};
    std::unordered_map<ConnectionId, std::shared_ptr<SmtpSession>, ConnectionIdHash> sessions_;
};

}
