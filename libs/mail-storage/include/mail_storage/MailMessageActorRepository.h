#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "Database.h"
#include "MailMessageActorRecord.h"

namespace Storage {

class Statement;

class MailMessageActorRepository
{
  public:
  explicit MailMessageActorRepository(Database& database);

  std::int64_t CreateActor(std::int64_t message_id,
                           const std::string& actor_email,
                           MailMessageActorType actor_type,
                           std::optional<DeliveryStatus> delivery_status = std::nullopt);

  std::optional<MailMessageActorRecord> FindById(std::int64_t actor_id) const;

  std::vector<MailMessageActorRecord> FindByMessageId(std::int64_t message_id) const;

  std::optional<MailMessageActorRecord> FindByMessageIdAndActorEmail(std::int64_t message_id,
                                                                     const std::string& actor_email) const;

  bool QueueActor(std::int64_t actor_id);

  std::vector<MailMessageActorRecord> ClaimReadyRecipients(int limit);

  bool MarkDelivered(std::int64_t actor_id);

  bool MarkTemporaryFailed(std::int64_t actor_id, const std::string& next_attempt_at, const std::string& last_error);

  bool MarkBounced(std::int64_t actor_id, const std::string& last_error);

  bool MarkFailed(std::int64_t actor_id, const std::string& last_error);

  bool SetStarred(std::int64_t message_id, const std::string& actor_email, bool starred);

  bool SetArchived(std::int64_t message_id, const std::string& actor_email, bool archived);

  bool MarkDeleted(std::int64_t message_id, const std::string& actor_email);

  private:
  Database& m_database;

  MailMessageActorRecord ReadActor(const Statement& statement) const;

  bool MarkTerminal(std::int64_t actor_id, DeliveryStatus status, const std::string& last_error);

  std::string ActorTypeToString(MailMessageActorType actor_type) const;

  MailMessageActorType ActorTypeFromString(const std::string& actor_type) const;

  std::string DeliveryStatusToString(DeliveryStatus status) const;

  DeliveryStatus DeliveryStatusFromString(const std::string& status) const;
};

} // namespace Storage
