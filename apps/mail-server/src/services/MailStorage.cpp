#include <algorithm>
#include <optional>
#include <utility>

#include <mail_server/services/MailStorage.h>
#include <mail_storage/MigrationRunner.h>

namespace ISXMailServer {

namespace {

bool IsRecipientActor(Storage::MailMessageActorType actor_type)
{
  return actor_type == Storage::MailMessageActorType::To || actor_type == Storage::MailMessageActorType::Cc ||
         actor_type == Storage::MailMessageActorType::Bcc;
}

} // namespace

MailStorage::MailStorage(const DatabaseConfiguration& configuration) :
  m_database(configuration.storage_path),
  m_messages(m_database),
  m_actors(m_database),
  m_users(m_database)
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
    const std::vector<Storage::MailMessageActorRecord> actors = m_actors.FindByMessageId(message.id);
    const auto current_actor =
      std::find_if(actors.begin(),
                   actors.end(),
                   [&user_email](const Storage::MailMessageActorRecord& actor)
                   { return actor.actor_email == user_email && !actor.deleted_at.has_value(); });

    if (current_actor != actors.end())
    {
      mails.push_back(SerializeMessage(message, *current_actor, actors));
    }
  }

  return mails;
}

boost::json::object MailStorage::CreateUser(const std::string& username,
                                            const std::string& email,
                                            const std::string& password_hash)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  const std::int64_t user_id = m_users.CreateUser(username, email, password_hash);
  const std::optional<Storage::UserRecord> user = m_users.FindById(user_id);

  return boost::json::object{{"id", user_id},
                             {"username", user.has_value() ? user->username : username},
                             {"email", user.has_value() ? user->email : email}};
}

std::optional<Storage::UserRecord> MailStorage::FindUserByEmail(const std::string& email)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_users.FindByEmail(email);
}

bool MailStorage::SetStarred(std::int64_t message_id, const std::string& actor_email, bool starred)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_actors.SetStarred(message_id, actor_email, starred);
}

bool MailStorage::SetArchived(std::int64_t message_id, const std::string& actor_email, bool archived)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_actors.SetArchived(message_id, actor_email, archived);
}

bool MailStorage::SetRead(std::int64_t message_id, const std::string& actor_email, bool read)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_actors.SetRead(message_id, actor_email, read);
}

bool MailStorage::DeleteMail(std::int64_t message_id, const std::string& actor_email)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_actors.MarkDeleted(message_id, actor_email);
}

boost::json::object MailStorage::SerializeMessage(const Storage::MailMessageRecord& message,
                                                  const Storage::MailMessageActorRecord& current_actor,
                                                  const std::vector<Storage::MailMessageActorRecord>& actors)
{
  boost::json::array recipients;
  std::string sender_email;

  for (const Storage::MailMessageActorRecord& actor : actors)
  {
    if (actor.actor_type == Storage::MailMessageActorType::From)
    {
      sender_email = actor.actor_email;
      continue;
    }

    if (IsRecipientActor(actor.actor_type))
    {
      recipients.push_back(boost::json::string(actor.actor_email));
    }
  }

  const bool is_sender = current_actor.actor_type == Storage::MailMessageActorType::From;
  const bool is_draft = message.message_status == Storage::MailMessageStatus::Draft;
  const bool is_archive = current_actor.archived_at.has_value();

  return boost::json::object{{"id", message.id},
                             {"from", sender_email},
                             {"to", std::move(recipients)},
                             {"subject", message.subject.value_or("")},
                             {"body", message.body},
                             {"created_at", message.created_at},
                             {"is_inbox", !is_sender && !is_draft && !is_archive},
                             {"is_sent", is_sender && !is_draft && !is_archive},
                             {"is_draft", is_draft},
                             {"is_starred", current_actor.starred_at.has_value()},
                             {"is_archive", is_archive},
                             {"is_read", current_actor.read_at.has_value()},
                             {"read_at", current_actor.read_at.value_or("")},
                             {"starred_at", current_actor.starred_at.value_or("")},
                             {"archived_at", current_actor.archived_at.value_or("")},
                             {"status", StatusToString(message.message_status)}};
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

} // namespace ISXMailServer
