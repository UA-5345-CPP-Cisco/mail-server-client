#include "headers/mail/MessageComposer.h"

#include <optional>

#include "headers/database/DatabaseManager.h"

namespace ISXMail
{

namespace
{
std::optional<std::string> ToOptionalString(const QString& text)
{
	const QString trimmed = text.trimmed();

	if (trimmed.isEmpty())
	{
		return std::nullopt;
	}

	return trimmed.toStdString();
}
}

MessageComposer::MessageComposer(QObject* parent) :
	QObject(parent),
	m_database(ISXDatabaseManager::DatabaseManager::DatabasePath()),
	m_repository(m_database),
	m_recipient_repository(m_database)
{
}

bool MessageComposer::SendMessage(
	const QString& sender_name,
	const QString& sender_email,
	const QString& recipient_email,
	const QString& subject,
	const QString& body,
	bool is_inbox
)
{
	Q_UNUSED(sender_name)

	if (recipient_email.trimmed().isEmpty() || body.trimmed().isEmpty())
	{
		return false;
	}

	m_database.Execute("BEGIN IMMEDIATE;");

	try
	{
		const std::int64_t message_id = m_repository.CreateMessage(
			std::nullopt,
			sender_email.toStdString(),
			ToOptionalString(subject),
			body.toStdString(),
			std::nullopt,
			is_inbox,
			Storage::MailMessageStatus::Sent
		);

		m_recipient_repository.CreateRecipient(
			message_id,
			recipient_email.toStdString(),
			Storage::RecipientType::To,
			Storage::DeliveryStatus::Pending
		);

		m_database.Execute("COMMIT;");
		return true;
	}
	catch (...)
	{
		try
		{
			m_database.Execute("ROLLBACK;");
		}
		catch (...)
		{
		}

		throw;
	}
}

bool MessageComposer::SaveDraft(
	const QString& sender_name,
	const QString& sender_email,
	const QString& recipient_email,
	const QString& subject,
	const QString& body
)
{
	Q_UNUSED(sender_name)

	if (recipient_email.trimmed().isEmpty() && subject.trimmed().isEmpty() && body.trimmed().isEmpty())
	{
		return false;
	}

	m_database.Execute("BEGIN IMMEDIATE;");

	try
	{
		const std::int64_t message_id = m_repository.CreateMessage(
			std::nullopt,
			sender_email.toStdString(),
			ToOptionalString(subject),
			body.toStdString(),
			std::nullopt,
            false,
			Storage::MailMessageStatus::Draft
		);

		if (!recipient_email.trimmed().isEmpty())
		{
			m_recipient_repository.CreateRecipient(
				message_id,
				recipient_email.toStdString(),
				Storage::RecipientType::To,
				Storage::DeliveryStatus::Pending
			);
		}

		m_database.Execute("COMMIT;");
		return true;
	}
	catch (...)
	{
		try
		{
			m_database.Execute("ROLLBACK;");
		}
		catch (...)
		{
		}

		throw;
	}
}

}
