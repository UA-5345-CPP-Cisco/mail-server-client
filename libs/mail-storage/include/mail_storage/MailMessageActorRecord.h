#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace Storage {

enum class MailMessageActorType
{
  From,
  To,
  Cc,
  Bcc
};

enum class DeliveryStatus
{
  Pending,
  Queued,
  Delivering,
  Delivered,
  TemporaryFailed,
  Bounced,
  Failed
};

struct MailMessageActorRecord
{
  std::int64_t id{};
  std::int64_t message_id{};
  std::string actor_email;
  MailMessageActorType actor_type{MailMessageActorType::To};
  std::optional<DeliveryStatus> delivery_status;
  int attempt_count{};
  std::optional<std::string> next_attempt_at;
  std::optional<std::string> last_error;
  std::optional<std::string> delivered_at;
  std::optional<std::string> read_at;
  std::optional<std::string> starred_at;
  std::optional<std::string> archived_at;
  std::optional<std::string> deleted_at;
};

} // namespace Storage
