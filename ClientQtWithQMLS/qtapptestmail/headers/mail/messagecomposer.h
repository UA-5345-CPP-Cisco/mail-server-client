#pragma once

#include <QObject>
#include <QString>

#include "headers/database/Database.h"
#include "headers/database/MailMessageRepository.h"
#include "headers/database/MessageRecipientRepository.h"

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
		const QString& body
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
