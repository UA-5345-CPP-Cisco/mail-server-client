#include "../include/mail_storage/MessageRecipientRepository.h"

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/mail_storage/Statement.h"

namespace Storage {

MessageRecipientRepository::MessageRecipientRepository(Database& database) : m_database(database)
{
}

std::int64_t MessageRecipientRepository::CreateRecipient(std::int64_t message_id,
                                                         const std::string& recipient_email,
                                                         RecipientType recipient_type,
                                                         DeliveryStatus delivery_status)
{
  Statement statement(m_database,
                      R"SQL(
			INSERT INTO message_recipients (
				message_id,
				recipient_email,
				recipient_type,
				delivery_status
			)
			VALUES (?, ?, ?, ?);
		)SQL");

  statement.BindInt64(1, message_id);
  statement.BindText(2, recipient_email);
  statement.BindText(3, RecipientTypeToString(recipient_type));
  statement.BindText(4, DeliveryStatusToString(delivery_status));
  statement.Step();

  return statement.LastInsertRowId();
}

std::optional<MessageRecipientRecord> MessageRecipientRepository::FindById(std::int64_t recipient_id) const
{
  Statement statement(m_database,
                      R"SQL(
			SELECT
				id,
				message_id,
				recipient_email,
				recipient_type,
				delivery_status,
				attempt_count,
				next_attempt_at,
				last_error,
				delivered_at
			FROM message_recipients
			WHERE id = ?
			LIMIT 1;
		)SQL");

  statement.BindInt64(1, recipient_id);

  if (!statement.Step())
  {
    return std::nullopt;
  }

  return ReadRecipient(statement);
}

std::vector<MessageRecipientRecord> MessageRecipientRepository::FindByMessageId(std::int64_t message_id) const
{
  Statement statement(m_database,
                      R"SQL(
			SELECT
				id,
				message_id,
				recipient_email,
				recipient_type,
				delivery_status,
				attempt_count,
				next_attempt_at,
				last_error,
				delivered_at
			FROM message_recipients
			WHERE message_id = ?
			ORDER BY id ASC;
		)SQL");

  statement.BindInt64(1, message_id);

  std::vector<MessageRecipientRecord> recipients;

  while (statement.Step())
  {
    recipients.push_back(ReadRecipient(statement));
  }

  return recipients;
}

bool MessageRecipientRepository::QueueRecipient(std::int64_t recipient_id)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE message_recipients
			SET
				delivery_status = 'queued',
				next_attempt_at = NULL,
				last_error = NULL
			WHERE id = ?
				AND delivery_status IN ('pending', 'temporary_failed');
		)SQL");

  statement.BindInt64(1, recipient_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

std::vector<MessageRecipientRecord> MessageRecipientRepository::ClaimReadyRecipients(int limit)
{
  if (limit <= 0)
  {
    return {};
  }

  Statement statement(m_database,
                      R"SQL(
			WITH ready_recipients AS (
				SELECT id
				FROM message_recipients
				WHERE
					delivery_status = 'queued'
					OR (
						delivery_status = 'temporary_failed'
						AND (
							next_attempt_at IS NULL
							OR next_attempt_at <= CURRENT_TIMESTAMP
						)
					)
				ORDER BY
					COALESCE(next_attempt_at, '1970-01-01 00:00:00') ASC,
					id ASC
				LIMIT ?
			)
			UPDATE message_recipients
			SET
				delivery_status = 'delivering',
				attempt_count = attempt_count + 1,
				next_attempt_at = NULL
			WHERE id IN (
				SELECT id
				FROM ready_recipients
			)
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
				recipient_email,
				recipient_type,
				delivery_status,
				attempt_count,
				next_attempt_at,
				last_error,
				delivered_at;
		)SQL");

  statement.BindInt(1, limit);

  std::vector<MessageRecipientRecord> recipients;

  while (statement.Step())
  {
    recipients.push_back(ReadRecipient(statement));
  }

  return recipients;
}

bool MessageRecipientRepository::MarkDelivered(std::int64_t recipient_id)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE message_recipients
			SET
				delivery_status = 'delivered',
				next_attempt_at = NULL,
				last_error = NULL,
				delivered_at = CURRENT_TIMESTAMP
			WHERE id = ?
				AND delivery_status = 'delivering';
		)SQL");

  statement.BindInt64(1, recipient_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

bool MessageRecipientRepository::MarkTemporaryFailed(std::int64_t recipient_id,
                                                     const std::string& next_attempt_at,
                                                     const std::string& last_error)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE message_recipients
			SET
				delivery_status = 'temporary_failed',
				next_attempt_at = ?,
				last_error = ?,
				delivered_at = NULL
			WHERE id = ?
				AND delivery_status = 'delivering';
		)SQL");

  statement.BindText(1, next_attempt_at);
  statement.BindText(2, last_error);
  statement.BindInt64(3, recipient_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

bool MessageRecipientRepository::MarkBounced(std::int64_t recipient_id, const std::string& last_error)
{
  return MarkTerminal(recipient_id, DeliveryStatus::Bounced, last_error);
}

bool MessageRecipientRepository::MarkFailed(std::int64_t recipient_id, const std::string& last_error)
{
  return MarkTerminal(recipient_id, DeliveryStatus::Failed, last_error);
}

MessageRecipientRecord MessageRecipientRepository::ReadRecipient(const Statement& statement) const
{
  MessageRecipientRecord recipient;
  recipient.id = statement.ColumnInt64(0);
  recipient.message_id = statement.ColumnInt64(1);
  recipient.recipient_email = statement.ColumnText(2);
  recipient.recipient_type = RecipientTypeFromString(statement.ColumnText(3));
  recipient.delivery_status = DeliveryStatusFromString(statement.ColumnText(4));
  recipient.attempt_count = static_cast<int>(statement.ColumnInt64(5));

  if (!statement.ColumnIsNull(6))
  {
    recipient.next_attempt_at = statement.ColumnText(6);
  }

  if (!statement.ColumnIsNull(7))
  {
    recipient.last_error = statement.ColumnText(7);
  }

  if (!statement.ColumnIsNull(8))
  {
    recipient.delivered_at = statement.ColumnText(8);
  }

  return recipient;
}

bool MessageRecipientRepository::MarkTerminal(std::int64_t recipient_id,
                                              DeliveryStatus status,
                                              const std::string& last_error)
{
  Statement statement(m_database,
                      R"SQL(
			UPDATE message_recipients
			SET
				delivery_status = ?,
				next_attempt_at = NULL,
				last_error = ?,
				delivered_at = NULL
			WHERE id = ?
				AND delivery_status = 'delivering';
		)SQL");

  statement.BindText(1, DeliveryStatusToString(status));
  statement.BindText(2, last_error);
  statement.BindInt64(3, recipient_id);
  statement.Step();

  return statement.ChangedRowCount() > 0;
}

std::string MessageRecipientRepository::RecipientTypeToString(RecipientType recipient_type) const
{
  switch (recipient_type)
  {
  case RecipientType::To:
    return "TO";
  case RecipientType::Cc:
    return "CC";
  case RecipientType::Bcc:
    return "BCC";
  }

  throw std::runtime_error("Unsupported recipient type");
}

RecipientType MessageRecipientRepository::RecipientTypeFromString(const std::string& recipient_type) const
{
  if (recipient_type == "TO")
  {
    return RecipientType::To;
  }

  if (recipient_type == "CC")
  {
    return RecipientType::Cc;
  }

  if (recipient_type == "BCC")
  {
    return RecipientType::Bcc;
  }

  throw std::runtime_error("Unsupported recipient type: " + recipient_type);
}

std::string MessageRecipientRepository::DeliveryStatusToString(DeliveryStatus status) const
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

DeliveryStatus MessageRecipientRepository::DeliveryStatusFromString(const std::string& status) const
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
