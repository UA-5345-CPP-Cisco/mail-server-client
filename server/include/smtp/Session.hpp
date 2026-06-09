#pragma once

#include "smtp/Event.hpp"
#include "smtp/Logger.hpp"
#include "smtp/Services.hpp"
#include "smtp/SocketsManager.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <queue>

namespace smtp {

struct SmtpSessionContext {
    ISocketsManager& socketsManager;
    IAuthService& authService;
    IMailStorage& mailStorage;
    ICacheService& cacheService;
    IDeliveryService& deliveryService;
    ILookupService& lookupService;
    ILogger& logger;
};

class ISmtpSessionHandler {
public:
    virtual ~ISmtpSessionHandler() = default;

    virtual void HandleEvent(const SmtpEvent& event, SmtpSessionContext& context) = 0;
};

class SmtpSession : public std::enable_shared_from_this<SmtpSession> {
public:
    SmtpSession(ConnectionId connectionId,
                SmtpSessionContext context,
                ISmtpSessionHandler& handler);

    ConnectionId Connection() const;

    // Called by the server loop thread after it receives socket events.
    void PushEvent(SmtpEvent event);

    // Gives the thread pool at most one task for this session at a time.
    bool TryExtractNextTask(std::function<void()>& task);
    bool IsReadyForRemoval() const;

private:
    void ProcessEvent(const SmtpEvent& event) noexcept;

    ConnectionId connectionId_;
    SmtpSessionContext context_;
    ISmtpSessionHandler& handler_;
    std::queue<SmtpEvent> events_;
    std::atomic_bool isProcessing_{false};
    std::atomic_bool isClosed_{false};
};

}
