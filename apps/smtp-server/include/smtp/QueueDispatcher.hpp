#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <mutex>

#include "logger/Logger.h"
#include "mail_storage/MailMessageRepository.h"
#include "mail_storage/MessageRecipientRepository.h"
#include "mail_storage/UserRepository.h"
#include "smtp/ServerConfig.hpp"
#include "thread_pool/ThreadPool.h"

namespace smtp {

class QueueDispatcher
{
  public:
  QueueDispatcher(DeliveryConfig config,
                  Concurrency::IThreadPool& threadPool,
                  Storage::UserRepository& users,
                  Storage::MailMessageRepository& mailMessages,
                  Storage::MessageRecipientRepository& messageRecipients,
                  std::mutex& storageMutex,
                  Logging::ILogger& logger);

  void Poll();

  private:
  static void Deliver(Storage::MessageRecipientRecord recipient,
                      Storage::UserRepository& users,
                      Storage::MailMessageRepository& mailMessages,
                      Storage::MessageRecipientRepository& messageRecipients,
                      std::mutex& storageMutex,
                      Logging::ILogger& logger) noexcept;

  DeliveryConfig config_;
  Concurrency::IThreadPool& threadPool_;
  Storage::UserRepository& users_;
  Storage::MailMessageRepository& mailMessages_;
  Storage::MessageRecipientRepository& messageRecipients_;
  std::mutex& storageMutex_;
  Logging::ILogger& logger_;
  std::chrono::steady_clock::time_point nextPoll_{};
};

} // namespace smtp
