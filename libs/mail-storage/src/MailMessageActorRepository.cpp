#include "../include/mail_storage/MailMessageActorRepository.h"

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/mail_storage/Statement.h"

namespace Storage {

MailMessageActorRepository::MailMessageActorRepository(Database& database) : m_database(database)
{
}

std::int64_t MailMessageActorRepository::CreateActor(std::int64_t message_id,
                                                     const std::string& actor_email,
                                                     MailMessageActorType actor_type,
                                                     std::optional<DeliveryStatus> delivery_status)
{
  if (!delivery_status.has_value() && actor_type != MailMessageActorType::From)
  {
    delivery_status = DeliveryStatus::Pending;
  }

  Statement statement(m_database,
                      R"SQL(
			INSERT INTO mail_message_actors (
				message_id,
				actor_email,
				actor_type,
				delivery_status
			)
			VALUES (?, ?, ?, ?);
		)SQL");

  statement.BindInt64(1, message_id);
  statement.BindText(2, actor_email);
  statement.BindText(3, ActorTypeToString(actor_type));

  if (delivery_status.has_value())
  {
    statement.BindText(4, DeliveryStatusToString(*delivery_status));
  }
  else
  {
    statement.BindNull(4);
  }

  statement.Step();

  return statement.LastInsertRowId();
}

std::optional<MailMessageActorRecord> MailMessageActorRepository::FindById(std::int64_t actor_id) const
{
  Statement statement(m_database,
                      R"SQL(
			SELECT
				id,
				message_id,
				actor_email,
				actor_type,
				delivery_status,
				attempt_count,
				next_attempt_at,
				last_error,
				delivered_at,
				read_at,
				starred_at,
				archived_at,
				deleted_at
			FROM mail_message_actors
			WHERE id = ?
			LIMIT 1;
		)SQL");

  statement.BindInt64(1, actor_id);

  if (!statement.Step())
  {
    return std::nullopt;
  }

  return ReadActor(statement);
}

std::vector<MailMessageActorRecord> MailMessageActorRepository::FindByMessageId(std::int64_t message_id) const
{
  Statement statement(m_database,
                      R"SQL(
			SELECT
				id,
				message_id,
				actor_email,
				actor_type,
				delivery_status,
				attempt_count,
				next_attempt_at,
				last_error,
				delivered_at,
				read_at,
				starred_at,
				archived_at,
				deleted_at
			FROM mail_message_actors
			WHERE message_id = ?
			ORDER BY id ASC;
		)SQL");

  statement.BindInt64(1, message_id);

  std::vector<MailMessageActorRecord> actors;

  while (statement.Step())
  {
    actors.push_back(ReadActor(statement));
  }

  return actors;
}

std::optional<MailMessageActorRecord> MailMessageActorRepository::FindByMessageIdAndActorEmail(
  std::int64_t message_id, const std::string& actor_email) const
{
  Statement statement(m_database,
                      R"SQL(
			SELECT
				id,
				message_id,
				actor_email,
				actor_type,
				delivery_status,
				attempt_count,
				next_attempt_at,
				last_error,
				delivered_at,
				read_at,
				starred_at,
				archived_at,
				deleted_at
			FROM mail_message_actors
			WHERE
				message_id = ?
				AND actor_email = ?
			ORDER BY
				CASE actor_type
					WHEN 'FROM' THEN 0
					WHEN 'TO' THEN 1
					WHEN 'CC' THEN 2
					WHEN 'BCC' THEN 3
					ELSE 4
				END,
				id ASC
			LIMIT 1;
		)SQL");

  statement.BindInt64(1, message_id);
  statement.BindText(2, actor_email);

  if (!statement.Step())
  {
    return std::nullopt;
  }

  return ReadActor(statement);
}

bool MailMessageActorRepository::QueueActor(std::int64_t actor_id)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_message_actors
			SET
				delivery_status = 'queued',
				next_attempt_at = NULL,
				last_error = NULL
			WHERE id = ?
				AND actor_type IN ('TO', 'CC', 'BCC')
				AND delivery_status IN ('pending', 'temporary_failed');
		)SQL");

  statement.BindInt64(1, actor_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

std::vector<MailMessageActorRecord> MailMessageActorRepository::ClaimReadyRecipients(int limit)
{
  if (limit <= 0)
  {
    return {};
  }

  Statement statement(m_database,
                      R"SQL(
			WITH ready_recipients AS (
				SELECT id
				FROM mail_message_actors
				WHERE
					actor_type IN ('TO', 'CC', 'BCC')
					AND (
						delivery_status = 'queued'
						OR (
							delivery_status = 'temporary_failed'
							AND (
								next_attempt_at IS NULL
								OR next_attempt_at <= CURRENT_TIMESTAMP
							)
						)
					)
				ORDER BY
					COALESCE(next_attempt_at, '1970-01-01 00:00:00') ASC,
					id ASC
				LIMIT ?
			)
			UPDATE mail_message_actors
			SET
				delivery_status = 'delivering',
				attempt_count = attempt_count + 1,
				next_attempt_at = NULL
			WHERE id IN (
				SELECT id
				FROM ready_recipients
			)
				AND actor_type IN ('TO', 'CC', 'BCC')
				AND (
					delivery_status = 'queued'
					OR (
						delivery_status = 'temporary_failed'
						AND (
							next_attempt_at IS NULL
							OR next_attempt_at <= CURRENT_TIMESTAMP
						)
					)
				)
			RETURNING
				id,
				message_id,
				actor_email,
				actor_type,
				delivery_status,
				attempt_count,
				next_attempt_at,
				last_error,
				delivered_at,
				read_at,
				starred_at,
				archived_at,
				deleted_at;
		)SQL");

  statement.BindInt(1, limit);

  std::vector<MailMessageActorRecord> actors;

  while (statement.Step())
  {
    actors.push_back(ReadActor(statement));
  }

  return actors;
}

bool MailMessageActorRepository::MarkDelivered(std::int64_t actor_id)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_message_actors
			SET
				delivery_status = 'delivered',
				next_attempt_at = NULL,
				last_error = NULL,
				delivered_at = CURRENT_TIMESTAMP
			WHERE id = ?
				AND actor_type IN ('TO', 'CC', 'BCC')
				AND delivery_status = 'delivering';
		)SQL");

  statement.BindInt64(1, actor_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

bool MailMessageActorRepository::MarkTemporaryFailed(std::int64_t actor_id,
                                                     const std::string& next_attempt_at,
                                                     const std::string& last_error)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_message_actors
			SET
				delivery_status = 'temporary_failed',
				next_attempt_at = ?,
				last_error = ?,
				delivered_at = NULL
			WHERE id = ?
				AND actor_type IN ('TO', 'CC', 'BCC')
				AND delivery_status = 'delivering';
		)SQL");

  statement.BindText(1, next_attempt_at);
  statement.BindText(2, last_error);
  statement.BindInt64(3, actor_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

bool MailMessageActorRepository::MarkBounced(std::int64_t actor_id, const std::string& last_error)
{
  return MarkTerminal(actor_id, DeliveryStatus::Bounced, last_error);
}

bool MailMessageActorRepository::MarkFailed(std::int64_t actor_id, const std::string& last_error)
{
  return MarkTerminal(actor_id, DeliveryStatus::Failed, last_error);
}

bool MailMessageActorRepository::SetStarred(std::int64_t message_id, const std::string& actor_email, bool starred)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_message_actors
			SET starred_at = CASE
				WHEN ?1 = 1 THEN COALESCE(starred_at, CURRENT_TIMESTAMP)
				ELSE NULL
			END
			WHERE
				message_id = ?2
				AND actor_email = ?3
				AND deleted_at IS NULL;
		)SQL");

  statement.BindInt(1, starred ? 1 : 0);
  statement.BindInt64(2, message_id);
  statement.BindText(3, actor_email);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

bool MailMessageActorRepository::SetArchived(std::int64_t message_id, const std::string& actor_email, bool archived)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_message_actors
			SET archived_at = CASE
				WHEN ?1 = 1 THEN COALESCE(archived_at, CURRENT_TIMESTAMP)
				ELSE NULL
			END
			WHERE
				message_id = ?2
				AND actor_email = ?3
				AND deleted_at IS NULL;
		)SQL");

  statement.BindInt(1, archived ? 1 : 0);
  statement.BindInt64(2, message_id);
  statement.BindText(3, actor_email);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

bool MailMessageActorRepository::MarkDeleted(std::int64_t message_id, const std::string& actor_email)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_message_actors
			SET deleted_at = COALESCE(deleted_at, CURRENT_TIMESTAMP)
			WHERE
				message_id = ?
				AND actor_email = ?
				AND deleted_at IS NULL;
		)SQL");

  statement.BindInt64(1, message_id);
  statement.BindText(2, actor_email);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

MailMessageActorRecord MailMessageActorRepository::ReadActor(const Statement& statement) const
{
  MailMessageActorRecord actor;
  actor.id = statement.ColumnInt64(0);
  actor.message_id = statement.ColumnInt64(1);
  actor.actor_email = statement.ColumnText(2);
  actor.actor_type = ActorTypeFromString(statement.ColumnText(3));

  if (!statement.ColumnIsNull(4))
  {
    actor.delivery_status = DeliveryStatusFromString(statement.ColumnText(4));
  }

  actor.attempt_count = static_cast<int>(statement.ColumnInt64(5));

  if (!statement.ColumnIsNull(6))
  {
    actor.next_attempt_at = statement.ColumnText(6);
  }

  if (!statement.ColumnIsNull(7))
  {
    actor.last_error = statement.ColumnText(7);
  }

  if (!statement.ColumnIsNull(8))
  {
    actor.delivered_at = statement.ColumnText(8);
  }

  if (!statement.ColumnIsNull(9))
  {
    actor.read_at = statement.ColumnText(9);
  }

  if (!statement.ColumnIsNull(10))
  {
    actor.starred_at = statement.ColumnText(10);
  }

  if (!statement.ColumnIsNull(11))
  {
    actor.archived_at = statement.ColumnText(11);
  }

  if (!statement.ColumnIsNull(12))
  {
    actor.deleted_at = statement.ColumnText(12);
  }

  return actor;
}

bool MailMessageActorRepository::MarkTerminal(std::int64_t actor_id,
                                              DeliveryStatus status,
                                              const std::string& last_error)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE mail_message_actors
			SET
				delivery_status = ?,
				next_attempt_at = NULL,
				last_error = ?,
				delivered_at = NULL
			WHERE id = ?
				AND actor_type IN ('TO', 'CC', 'BCC')
				AND delivery_status = 'delivering';
		)SQL");

  statement.BindText(1, DeliveryStatusToString(status));
  statement.BindText(2, last_error);
  statement.BindInt64(3, actor_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

std::string MailMessageActorRepository::ActorTypeToString(MailMessageActorType actor_type) const
{
  switch (actor_type)
  {
  case MailMessageActorType::From:
    return "FROM";
  case MailMessageActorType::To:
    return "TO";
  case MailMessageActorType::Cc:
    return "CC";
  case MailMessageActorType::Bcc:
    return "BCC";
  }

  throw std::runtime_error("Unsupported actor type");
}

MailMessageActorType MailMessageActorRepository::ActorTypeFromString(const std::string& actor_type) const
{
  if (actor_type == "FROM")
  {
    return MailMessageActorType::From;
  }

  if (actor_type == "TO")
  {
    return MailMessageActorType::To;
  }

  if (actor_type == "CC")
  {
    return MailMessageActorType::Cc;
  }

  if (actor_type == "BCC")
  {
    return MailMessageActorType::Bcc;
  }

  throw std::runtime_error("Unsupported actor type: " + actor_type);
}

std::string MailMessageActorRepository::DeliveryStatusToString(DeliveryStatus status) const
{
  switch (status)
  {
  case DeliveryStatus::Pending:
    return "pending";
  case DeliveryStatus::Queued:
    return "queued";
  case DeliveryStatus::Delivering:
    return "delivering";
  case DeliveryStatus::Delivered:
    return "delivered";
  case DeliveryStatus::TemporaryFailed:
    return "temporary_failed";
  case DeliveryStatus::Bounced:
    return "bounced";
  case DeliveryStatus::Failed:
    return "failed";
  }

  throw std::runtime_error("Unsupported delivery status");
}

DeliveryStatus MailMessageActorRepository::DeliveryStatusFromString(const std::string& status) const
{
  if (status == "pending")
  {
    return DeliveryStatus::Pending;
  }

  if (status == "queued")
  {
    return DeliveryStatus::Queued;
  }

  if (status == "delivering")
  {
    return DeliveryStatus::Delivering;
  }

  if (status == "delivered")
  {
    return DeliveryStatus::Delivered;
  }

  if (status == "temporary_failed")
  {
    return DeliveryStatus::TemporaryFailed;
  }

  if (status == "bounced")
  {
    return DeliveryStatus::Bounced;
  }

  if (status == "failed")
  {
    return DeliveryStatus::Failed;
  }

  throw std::runtime_error("Unsupported delivery status: " + status);
}

} // namespace Storage
