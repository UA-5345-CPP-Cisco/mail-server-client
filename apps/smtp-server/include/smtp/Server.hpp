#pragma once

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "logger/Logger.h"
#include "mail_storage/Database.h"
#include "mail_storage/MailMessageRepository.h"
#include "mail_storage/MessageRecipientRepository.h"
#include "smtp/AuthService.hpp"
#include "smtp/Event.hpp"
#include "smtp/QueueDispatcher.hpp"
#include "smtp/ServerConfig.hpp"
#include "smtp/Session.hpp"
#include "smtp/SocketsManager.hpp"
#include "thread_pool/ThreadPool.h"

namespace smtp {

// References to infrastructure and service components used by the server.
// SmtpServer coordinates these objects but does not own them.
struct SmtpServerDependencies
{
  ISocketsManager& socketsManager;
  Concurrency::IThreadPool& threadPool;
  SmtpSessionHandler& sessionHandler;
  IAuthService& authService;
  Storage::Database& database;
  Storage::MailMessageRepository& mailMessages;
  Storage::MessageRecipientRepository& messageRecipients;
  std::mutex& storageMutex;
  QueueDispatcher& queueDispatcher;
  Logging::ILogger& logger;
};

class SmtpServer
{
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

} // namespace smtp
