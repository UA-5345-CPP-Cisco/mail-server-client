#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "Database.h"
#include "MailMessageRecord.h"

namespace Storage {

class Statement;

class MailMessageRepository
{
public:
  explicit MailMessageRepository(Database& database);

  std::int64_t CreateMessage(const std::optional<std::int64_t>& sender_user_id,
                             const std::string& sender_email,
                             const std::optional<std::string>& subject,
                             const std::string& body,
                             const std::optional<std::int64_t>& reply_to_message_id,
                             bool is_inbox,
                             MailMessageStatus status = MailMessageStatus::Queued);

  std::optional<MailMessageRecord> FindById(std::int64_t message_id) const;

  std::vector<MailMessageRecord> FindByStatus(MailMessageStatus status, int limit) const;

  bool UpdateStatus(std::int64_t message_id,
                    MailMessageStatus expected_status,
                    MailMessageStatus new_status);

  bool FinalizeDelivery(std::int64_t message_id);

  std::vector<MailMessageRecord> FindAll() const;

  bool UpdateStarred(std::int64_t message_id, bool starred);

  bool UpdateArchive(std::int64_t message_id, bool archive);

  int DeleteArchivedOlderThanDays(int days);

  bool DeleteMessage(std::int64_t message_id);

private:
  Database& m_database;

  MailMessageRecord ReadMessage(const Statement& statement) const;

  std::string StatusToString(MailMessageStatus status) const;

  MailMessageStatus StatusFromString(const std::string& status) const;
};

} // namespace Storage
