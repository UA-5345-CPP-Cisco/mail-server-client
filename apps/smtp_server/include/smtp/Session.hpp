#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <vector>

#include "logger/Logger.h"
#include "smtp/AuthService.hpp"
#include "smtp/Event.hpp"
#include "smtp/ServerConfig.hpp"
#include "smtp/SocketsManager.hpp"
#include "storage/Database.h"
#include "storage/MailMessageRepository.h"
#include "storage/MessageRecipientRepository.h"

namespace smtp {

// Shared dependencies available while processing one SMTP session event.
struct SmtpSessionContext
{
  const ServerConfig& config;
  ISocketsManager& socketsManager;
  IAuthService& authService;
  Storage::Database& database;
  Storage::MailMessageRepository& mailMessages;
  Storage::MessageRecipientRepository& messageRecipients;
  std::mutex& storageMutex;
  Logging::ILogger& logger;
};

// High-level state of the SMTP conversation for one client.
enum class SmtpSessionPhase
{
  WaitingForGreeting,
  ReadyForMail,
  MailTransaction,
  ReceivingMessage,
  Closing,
  Closed
};

enum class SmtpAuthStage
{
  None,
  PlainResponse,
  LoginUsername,
  LoginPassword
};

// Mutable protocol state owned by one SmtpSession.
// SmtpSessionHandler updates this while processing queued events.
struct SmtpSessionState
{
  explicit SmtpSessionState(ConnectionId connectionId) : connectionId(connectionId)
  {
  }

  ConnectionId connectionId;
  SmtpSessionPhase phase{SmtpSessionPhase::WaitingForGreeting};
  bool tlsActive{false};
  bool tlsHandshakePending{false};
  bool authenticated{false};
  SmtpAuthStage authStage{SmtpAuthStage::None};
  std::string clientName;
  std::string authenticatedIdentity;
  std::string pendingAuthUsername;
  std::string sender;
  std::vector<std::string> recipients;
  std::string messageBuffer;
};

class SmtpSessionHandler
{
  public:
  void HandleEvent(const SmtpEvent& event, SmtpSessionState& state, SmtpSessionContext& context);

  private:
  void
  HandleConnected(const SmtpEvent& event, SmtpSessionState& state, SmtpSessionContext& context);

  void HandleMessageReceived(const SmtpEvent& event,
                             SmtpSessionState& state,
                             SmtpSessionContext& context);

  void
  HandleDisconnected(const SmtpEvent& event, SmtpSessionState& state, SmtpSessionContext& context);

  std::int64_t StoreMessage(const SmtpSessionState& state, SmtpSessionContext& context);

  void ResetMailTransaction(SmtpSessionState& state);

  void SendReply(SmtpSessionContext& context,
                 ConnectionId connectionId,
                 int statusCode,
                 const std::string& message);
};

// Owns one client's ordered event queue. The server loop pushes events here;
// the thread pool receives tasks produced by TryExtractNextTask().
class SmtpSession : public std::enable_shared_from_this<SmtpSession>
{
  public:
  SmtpSession(ConnectionId connectionId, SmtpSessionContext context, SmtpSessionHandler& handler);

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

} // namespace smtp
