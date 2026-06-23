#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "storage/Database.h"
#include "storage/MessageRecipientRecord.h"

namespace Storage {

class Statement;

class MessageRecipientRepository
{
  public:
  explicit MessageRecipientRepository(Database& database);

  std::int64_t CreateRecipient(std::int64_t message_id,
                               const std::string& recipient_email,
                               RecipientType recipient_type,
                               DeliveryStatus delivery_status = DeliveryStatus::Pending);

  std::optional<MessageRecipientRecord> FindById(std::int64_t recipient_id) const;
  std::vector<MessageRecipientRecord> FindByMessageId(std::int64_t message_id) const;

  bool QueueRecipient(std::int64_t recipient_id);
  std::vector<MessageRecipientRecord> ClaimReadyRecipients(int limit);
  bool MarkDelivered(std::int64_t recipient_id);
  bool MarkTemporaryFailed(std::int64_t recipient_id,
                           const std::string& next_attempt_at,
                           const std::string& last_error);
  bool MarkBounced(std::int64_t recipient_id, const std::string& last_error);
  bool MarkFailed(std::int64_t recipient_id, const std::string& last_error);

  private:
  Database& m_database;

  MessageRecipientRecord ReadRecipient(const Statement& statement) const;
  bool
  MarkTerminal(std::int64_t recipient_id, DeliveryStatus status, const std::string& last_error);
  std::string RecipientTypeToString(RecipientType recipient_type) const;
  RecipientType RecipientTypeFromString(const std::string& recipient_type) const;
  std::string DeliveryStatusToString(DeliveryStatus status) const;
  DeliveryStatus DeliveryStatusFromString(const std::string& status) const;
};

} // namespace Storage
