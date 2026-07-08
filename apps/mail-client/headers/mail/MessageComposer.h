#pragma once

#include <QObject>
#include <QString>

#include "../../../../libs/mail-storage/include/mail_storage/Database.h"
#include "../../../../libs/mail-storage/include/mail_storage/MailMessageRepository.h"
#include "../../../../libs/mail-storage/include/mail_storage/MessageRecipientRepository.h"

namespace ISXMail
{

class MessageComposer : public QObject
{
	Q_OBJECT

public:
	explicit MessageComposer(QObject* parent = nullptr);

	Q_INVOKABLE bool SendMessage(
		const QString& sender_name,
		const QString& sender_email,
		const QString& recipient_email,
		const QString& subject,
		const QString& body,
		bool is_inbox = false
	);
	Q_INVOKABLE bool SaveDraft(
		const QString& sender_name,
		const QString& sender_email,
		const QString& recipient_email,
		const QString& subject,
		const QString& body
	);

private:
	Storage::Database m_database;
	Storage::MailMessageRepository m_repository;
	Storage::MessageRecipientRepository m_recipient_repository;

	void EnsureSchema();
};

}
