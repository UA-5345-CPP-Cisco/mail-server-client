#pragma once

#include "smtp/Event.hpp"
#include "smtp/Logger.hpp"
#include "smtp/Services.hpp"
#include "smtp/SocketsManager.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace smtp {

// Shared dependencies available while processing one SMTP session event.
struct SmtpSessionContext {
    ISocketsManager& socketsManager;
    IAuthService& authService;
    IMailStorage& mailStorage;
    ICacheService& cacheService;
    IDeliveryService& deliveryService;
    ILookupService& lookupService;
    ILogger& logger;
};

// High-level state of the SMTP conversation for one client.
enum class SmtpSessionPhase {
    WaitingForGreeting,
    ReadyForMail,
    MailTransaction,
    ReceivingMessage,
    Closing,
    Closed
};

// Mutable protocol state owned by one SmtpSession.
// SmtpSessionHandler updates this while processing queued events.
struct SmtpSessionState {
    ConnectionId connectionId;
    SmtpSessionPhase phase{SmtpSessionPhase::WaitingForGreeting};
    bool tlsActive{false};
    bool authenticated{false};
    std::string clientName;
    std::string sender;
    std::vector<std::string> recipients;
    std::string messageBuffer;
};

// Concrete SMTP protocol component. This class is responsible for interpreting
// session events: greeting, command parsing, transaction state, STARTTLS handoff,
// auth/storage/cache/delivery/lookup calls, and socket responses.
class SmtpSessionHandler {
public:
    void HandleEvent(const SmtpEvent& event,
                     SmtpSessionState& state,
                     SmtpSessionContext& context);

private:
    void HandleConnected(const SmtpEvent& event,
                         SmtpSessionState& state,
                         SmtpSessionContext& context);
    void HandleMessageReceived(const SmtpEvent& event,
                               SmtpSessionState& state,
                               SmtpSessionContext& context);
    void HandleDisconnected(const SmtpEvent& event,
                            SmtpSessionState& state,
                            SmtpSessionContext& context);
    void ResetMailTransaction(SmtpSessionState& state);
    void SendReply(SmtpSessionContext& context,
                   ConnectionId connectionId,
                   int statusCode,
                   const std::string& message);
};

// Owns one client's ordered event queue. The server loop pushes events here;
// the thread pool receives tasks produced by TryExtractNextTask().
class SmtpSession : public std::enable_shared_from_this<SmtpSession> {
public:
    SmtpSession(ConnectionId connectionId,
                SmtpSessionContext context,
                SmtpSessionHandler& handler);

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
    SmtpSessionHandler& handler_;
    SmtpSessionState state_;
    std::queue<SmtpEvent> events_;
    std::atomic_bool isProcessing_{false};
    std::atomic_bool isClosed_{false};
};

}
