#include <optional>

#include <QtConcurrent>
#include <QFutureWatcher>

#include "headers/mail/MessageComposer.h"
#include "headers/database/DatabaseManager.h"
#include "headers/service/Service.h"

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
	 ISXService::Service::Logger().Log(Logging::LogLevel::Info, std::string("MessageComposer: opened DB at ") + ISXDatabaseManager::DatabaseManager::DatabasePath().string());
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
	Q_UNUSED(sender_name);

	if (recipient_email.trimmed().isEmpty() || body.trimmed().isEmpty())
	{
			 ISXService::Service::Logger().Log(Logging::LogLevel::Warning, "MessageComposer::SendMessage: validation failed - recipient or body empty");
			return false;
		}

		 ISXService::Service::Logger().Log(Logging::LogLevel::Info, (std::string("MessageComposer::SendMessage: sending to ") + recipient_email.toStdString() + " subject_len=" + std::to_string(subject.size())));
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
		 ISXService::Service::Logger().Log(Logging::LogLevel::Info, (std::string("MessageComposer::SendMessage: committed message_id=") + std::to_string(message_id)));
	  const std::string host = m_smtp_host.toStdString();
	  const uint16_t port = m_smtp_port;
	  const std::string smtp_sender = sender_email.toStdString();
	  const std::string smtp_recipient = recipient_email.trimmed().toStdString();
	  const std::string smtp_subject = subject.toStdString();
	  const std::string smtp_body = body.toStdString();

	  auto future = QtConcurrent::run([=]() -> Smtp::SmtpResult {
      Smtp::SmtpClient client(host, port);
      Smtp::Mail mail = Smtp::MailBuilder()
        .SetSender(smtp_sender)
        .AddRecipients({smtp_recipient})
        .SetSubject(smtp_subject)
        .SetBody(smtp_body)
        .Build();
      return client.SendMailMessage(mail);
    });

	  auto* watcher = new QFutureWatcher<Smtp::SmtpResult>(this);
	  connect(watcher, &QFutureWatcher<Smtp::SmtpResult>::finished,
        this, [this, watcher]() {
      const auto result = watcher->result();
      emit messageSent(result.ok(),
               QString::fromStdString(result.message));
      watcher->deleteLater();
    });
	  watcher->setFuture(future);

	  return true;

	}
	catch (...)
	{
		 ISXService::Service::Logger().Log(Logging::LogLevel::Error, "MessageComposer::SendMessage: exception occurred, attempting ROLLBACK");
		try
		{
			m_database.Execute("ROLLBACK;");
			 ISXService::Service::Logger().Log(Logging::LogLevel::Info, "MessageComposer::SendMessage: rollback succeeded");
		}
		catch (...)
		{
			 ISXService::Service::Logger().Log(Logging::LogLevel::Error, "MessageComposer::SendMessage: rollback failed");
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
	Q_UNUSED(sender_name);

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
		 ISXService::Service::Logger().Log(Logging::LogLevel::Error, "MessageComposer::SaveDraft: exception occurred, attempting ROLLBACK");
		try
		{
			m_database.Execute("ROLLBACK;");
			 ISXService::Service::Logger().Log(Logging::LogLevel::Info, "MessageComposer::SaveDraft: rollback succeeded");
		}
		catch (...)
		{
			 ISXService::Service::Logger().Log(Logging::LogLevel::Error, "MessageComposer::SaveDraft: rollback failed");
		}

		throw;
	}
}

}
