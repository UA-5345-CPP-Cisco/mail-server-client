#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace Storage {

enum class RecipientType
{
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

struct MessageRecipientRecord
{
  std::int64_t id{};
  std::int64_t message_id{};
  std::string recipient_email;
  RecipientType recipient_type{RecipientType::To};
  DeliveryStatus delivery_status{DeliveryStatus::Pending};
  int attempt_count{};
  std::optional<std::string> next_attempt_at;
  std::optional<std::string> last_error;
  std::optional<std::string> delivered_at;
};

} // namespace Storage
