#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace Storage {

enum class MailMessageStatus
{
  Draft,
  Queued,
  Sending,
  Sent,
  Failed
};

struct MailMessageRecord
{
  std::int64_t id{};
  std::optional<std::int64_t> sender_user_id;
  std::string sender_email;
  std::optional<std::string> subject;
  std::string body;
  std::optional<std::int64_t> reply_to_message_id;
  std::string created_at;
  bool is_inbox{false};
  bool is_starred{false};
  bool is_draft{false}; // move into status
  MailMessageStatus status{MailMessageStatus::Queued};
};

} // namespace Storage
