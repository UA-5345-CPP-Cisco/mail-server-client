#include "../include/mail_storage/MailMessageRepository.h"

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/mail_storage/Statement.h"

namespace Storage {

MailMessageRepository::MailMessageRepository(Database& database) : m_database(database)
{
}

std::int64_t MailMessageRepository::CreateMessage(const std::optional<std::string>& subject,
                                                  const std::string& body,
                                                  const std::optional<std::int64_t>& reply_to_message_id,
                                                  MailMessageStatus status)
{
  Statement statement(m_database,
                      R"SQL(
			INSERT INTO mail_messages (
				subject,
				body,
				reply_to_message_id,
				message_status
			)
			VALUES (?, ?, ?, ?);
		)SQL");

  if (subject.has_value())
  {
    statement.BindText(1, *subject);
  }
  else
  {
    statement.BindNull(1);
  }

  statement.BindText(2, body);

  if (reply_to_message_id.has_value())
  {
    statement.BindInt64(3, *reply_to_message_id);
  }
  else
  {
    statement.BindNull(3);
  }

  statement.BindText(4, StatusToString(status));
  statement.Step();

  return statement.LastInsertRowId();
}

std::optional<MailMessageRecord> MailMessageRepository::FindById(std::int64_t message_id) const
{
  Statement statement(m_database,
                      R"SQL(
			SELECT
				id,
				subject,
				body,
				reply_to_message_id,
				created_at,
				message_status
			FROM mail_messages
			WHERE id = ?
			LIMIT 1;
		)SQL");

  statement.BindInt64(1, message_id);

  if (!statement.Step())
  {
    return std::nullopt;
  }

  return ReadMessage(statement);
}

std::vector<MailMessageRecord> MailMessageRepository::FindAll() const
{
  Statement statement(m_database,
                      R"SQL(
			SELECT
				id,
				subject,
				body,
				reply_to_message_id,
				created_at,
				message_status
			FROM mail_messages
			ORDER BY created_at DESC, id DESC;
		)SQL");

  std::vector<MailMessageRecord> messages;

  while (statement.Step())
  {
    messages.push_back(ReadMessage(statement));
  }

  return messages;
}

std::vector<MailMessageRecord> MailMessageRepository::FindByStatus(MailMessageStatus status, int limit) const
{
  if (limit <= 0)
  {
    return {};
  }

  Statement statement(m_database,
                      R"SQL(
			SELECT
				id,
				subject,
				body,
				reply_to_message_id,
				created_at,
				message_status
			FROM mail_messages
			WHERE message_status = ?
			ORDER BY created_at ASC, id ASC
			LIMIT ?;
		)SQL");

  statement.BindText(1, StatusToString(status));
  statement.BindInt(2, limit);

  std::vector<MailMessageRecord> messages;

  while (statement.Step())
  {
    messages.push_back(ReadMessage(statement));
  }

  return messages;
}

bool MailMessageRepository::UpdateStatus(std::int64_t message_id,
                                         MailMessageStatus expected_status,
                                         MailMessageStatus new_status)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_messages
			SET message_status = ?
			WHERE id = ?
				AND message_status = ?;
		)SQL");

  statement.BindText(1, StatusToString(new_status));
  statement.BindInt64(2, message_id);
  statement.BindText(3, StatusToString(expected_status));
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

bool MailMessageRepository::DeleteMessage(std::int64_t message_id)
{
  Statement statement(m_database,
                      R"SQL(
			DELETE FROM mail_messages
			WHERE id = ?;
		)SQL");

  statement.BindInt64(1, message_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

bool MailMessageRepository::FinalizeDelivery(std::int64_t message_id)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_messages
			SET message_status = CASE
				WHEN EXISTS (
					SELECT 1
					FROM mail_message_actors
					WHERE
						message_id = ?
						AND actor_type IN ('TO', 'CC', 'BCC')
						AND delivery_status = 'delivered'
				)
				THEN 'sent'
				ELSE 'failed'
			END
			WHERE
				id = ?
				AND message_status IN ('queued', 'sending')
				AND NOT EXISTS (
					SELECT 1
					FROM mail_message_actors
					WHERE
						message_id = ?
						AND actor_type IN ('TO', 'CC', 'BCC')
						AND delivery_status IN (
							'pending',
							'queued',
							'delivering',
							'temporary_failed'
						)
				);
		)SQL");

  statement.BindInt64(1, message_id);
  statement.BindInt64(2, message_id);
  statement.BindInt64(3, message_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

MailMessageRecord MailMessageRepository::ReadMessage(const Statement& statement) const
{
  MailMessageRecord message;
  message.id = statement.ColumnInt64(0);

  if (!statement.ColumnIsNull(1))
  {
    message.subject = statement.ColumnText(1);
  }

  message.body = statement.ColumnText(2);

  if (!statement.ColumnIsNull(3))
  {
    message.reply_to_message_id = statement.ColumnInt64(3);
  }

  message.created_at = statement.ColumnText(4);
  message.message_status = StatusFromString(statement.ColumnText(5));

  return message;
}

std::string MailMessageRepository::StatusToString(MailMessageStatus status) const
{
  switch (status)
  {
  case MailMessageStatus::Draft:
    return "draft";
  case MailMessageStatus::Queued:
    return "queued";
  case MailMessageStatus::Sending:
    return "sending";
  case MailMessageStatus::Sent:
    return "sent";
  case MailMessageStatus::Failed:
    return "failed";
  }

  throw std::runtime_error("Unsupported mail message status");
}

MailMessageStatus MailMessageRepository::StatusFromString(const std::string& status) const
{
  if (status == "draft")
  {
    return MailMessageStatus::Draft;
  }

  if (status == "queued")
  {
    return MailMessageStatus::Queued;
  }

  if (status == "sending")
  {
    return MailMessageStatus::Sending;
  }

  if (status == "sent")
  {
    return MailMessageStatus::Sent;
  }

  if (status == "failed")
  {
    return MailMessageStatus::Failed;
  }

  throw std::runtime_error("Unsupported mail message status: " + status);
}

} // namespace Storage
