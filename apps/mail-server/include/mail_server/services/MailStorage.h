#pragma once

#include <mutex>
#include <string>
#include <vector>

#include <boost/json.hpp>
#include <mail_server/Configuration.h>
#include <mail_storage/Database.h>
#include <mail_storage/MailMessageRepository.h>
#include <mail_storage/MessageRecipientRepository.h>
#include <mail_storage/UserRepository.h>

namespace ISXMailServer {

struct SendMailRequest
{
  std::string from;
  std::vector<std::string> to;
  std::string subject;
  std::string body;
};

class MailStorage
{
  public:
  explicit MailStorage(const DatabaseConfiguration& configuration);

  [[nodiscard]] boost::json::array FindMailsForUser(const std::string& user_email);
  [[nodiscard]] boost::json::object
  CreateUser(const std::string& username, const std::string& email, const std::string& password_hash);
  [[nodiscard]] std::optional<Storage::UserRecord> FindUserByEmail(const std::string& email);

  private:
  Storage::Database m_database;
  Storage::MailMessageRepository m_messages;
  Storage::MessageRecipientRepository m_recipients;
  Storage::UserRepository m_users;
  std::mutex m_mutex;

  [[nodiscard]] boost::json::object SerializeMessage(const Storage::MailMessageRecord& message);
  [[nodiscard]] std::string StatusToString(Storage::MailMessageStatus status) const;
  [[nodiscard]] bool HasRecipient(const Storage::MailMessageRecord& message, const std::string& user_email);
};

} // namespace ISXMailServer
