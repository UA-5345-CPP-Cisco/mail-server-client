#include "smtp/QueueDispatcher.hpp"

#include <exception>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace smtp {

QueueDispatcher::QueueDispatcher(DeliveryConfig config,
                                 Concurrency::IThreadPool& threadPool,
                                 Storage::UserRepository& users,
                                 Storage::MailMessageRepository& mailMessages,
                                 Storage::MailMessageActorRepository& messageActors,
                                 std::mutex& storageMutex,
                                 Logging::ILogger& logger) :
  config_(config),
  threadPool_(threadPool),
  users_(users),
  mailMessages_(mailMessages),
  messageActors_(messageActors),
  storageMutex_(storageMutex),
  logger_(logger)
{
  if (config_.batchSize == 0)
  {
    throw std::invalid_argument("QueueDispatcher batch size must be greater than zero");
  }
}

void QueueDispatcher::Poll()
{
  const auto now = std::chrono::steady_clock::now();
  if (now < nextPoll_)
  {
    return;
  }
  nextPoll_ = now + std::chrono::milliseconds(config_.pollIntervalMilliseconds);

  const std::size_t maximumInt = static_cast<std::size_t>(std::numeric_limits<int>::max());
  const int limit = static_cast<int>(config_.batchSize > maximumInt ? maximumInt : config_.batchSize);

  std::vector<Storage::MailMessageActorRecord> recipients;
  {
    std::lock_guard<std::mutex> lock(storageMutex_);
    recipients = messageActors_.ClaimReadyRecipients(limit);

    for (const Storage::MailMessageActorRecord& recipient : recipients)
    {
      mailMessages_.UpdateStatus(
        recipient.message_id, Storage::MailMessageStatus::Queued, Storage::MailMessageStatus::Sending);
    }
  }

  for (const Storage::MailMessageActorRecord& recipient : recipients)
  {
    try
    {
      threadPool_.Enqueue([recipient,
                           &users = users_,
                           &mailMessages = mailMessages_,
                           &messageActors = messageActors_,
                           &storageMutex = storageMutex_,
                           &logger = logger_]
                          { Deliver(recipient, users, mailMessages, messageActors, storageMutex, logger); });
    }
    catch (const std::exception& error)
    {
      std::lock_guard<std::mutex> lock(storageMutex_);
      messageActors_.MarkFailed(recipient.id, error.what());
      mailMessages_.FinalizeDelivery(recipient.message_id);
    }
  }
}

void QueueDispatcher::Deliver(Storage::MailMessageActorRecord recipient,
                              Storage::UserRepository& users,
                              Storage::MailMessageRepository& mailMessages,
                              Storage::MailMessageActorRepository& messageActors,
                              std::mutex& storageMutex,
                              Logging::ILogger& logger) noexcept
{
  try
  {
    std::lock_guard<std::mutex> lock(storageMutex);
    const std::optional<Storage::UserRecord> user = users.FindByEmail(recipient.actor_email);

    if (user && user->status == Storage::UserStatus::Active)
    {
      messageActors.MarkDelivered(recipient.id);
    }
    else
    {
      messageActors.MarkFailed(recipient.id, "Recipient is not an active local user");
    }

    mailMessages.FinalizeDelivery(recipient.message_id);
  }
  catch (const std::exception& error)
  {
    logger.Log(Logging::LogLevel::Error, error.what());
    try
    {
      std::lock_guard<std::mutex> lock(storageMutex);
      messageActors.MarkFailed(recipient.id, error.what());
      mailMessages.FinalizeDelivery(recipient.message_id);
    }
    catch (...)
    {
    }
  }
  catch (...)
  {
    logger.Log(Logging::LogLevel::Error, "Unhandled local delivery error");
    try
    {
      std::lock_guard<std::mutex> lock(storageMutex);
      messageActors.MarkFailed(recipient.id, "Unhandled local delivery error");
      mailMessages.FinalizeDelivery(recipient.message_id);
    }
    catch (...)
    {
    }
  }
}

} // namespace smtp
