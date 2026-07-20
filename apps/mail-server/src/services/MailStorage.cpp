#include <algorithm>
#include <utility>

#include <mail_server/services/MailStorage.h>
#include <mail_storage/MigrationRunner.h>

namespace ISXMailServer {

MailStorage::MailStorage(const DatabaseConfiguration& configuration) :
  m_database(configuration.storage_path),
  m_messages(m_database),
  m_recipients(m_database)
{
  Storage::MigrationRunner runner(m_database, configuration.migrations_path);
  runner.Run();
}

boost::json::array MailStorage::FindMailsForUser(const std::string& user_email)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  boost::json::array mails;
  for (const Storage::MailMessageRecord& message : m_messages.FindAll())
  {
    if (message.sender_email == user_email || HasRecipient(message, user_email))
    {
      mails.push_back(SerializeMessage(message));
    }
  }

  return mails;
}

boost::json::object MailStorage::SerializeMessage(const Storage::MailMessageRecord& message)
{
  boost::json::array recipients;
  for (const Storage::MessageRecipientRecord& recipient : m_recipients.FindByMessageId(message.id))
  {
    recipients.push_back(boost::json::string(recipient.recipient_email));
  }

  return boost::json::object{{"id", message.id},
                             {"from", message.sender_email},
                             {"to", std::move(recipients)},
                             {"subject", message.subject.value_or("")},
                             {"body", message.body},
                             {"created_at", message.created_at},
                             {"is_inbox", message.is_inbox},
                             {"is_starred", message.is_starred},
                             {"status", StatusToString(message.status)}};
}

std::string MailStorage::StatusToString(Storage::MailMessageStatus status) const
{
  switch (status)
  {
  case Storage::MailMessageStatus::Draft:
    return "draft";
  case Storage::MailMessageStatus::Queued:
    return "queued";
  case Storage::MailMessageStatus::Sending:
    return "sending";
  case Storage::MailMessageStatus::Sent:
    return "sent";
  case Storage::MailMessageStatus::Failed:
    return "failed";
  }

  return "unknown";
}

bool MailStorage::HasRecipient(const Storage::MailMessageRecord& message, const std::string& user_email)
{
  const std::vector<Storage::MessageRecipientRecord> recipients = m_recipients.FindByMessageId(message.id);
  return std::any_of(recipients.begin(),
                     recipients.end(),
                     [&user_email](const Storage::MessageRecipientRecord& recipient)
                     { return recipient.recipient_email == user_email; });
}

} // namespace ISXMailServer
