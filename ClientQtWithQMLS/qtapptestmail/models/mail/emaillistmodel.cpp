#include "headers/mail/emaillistmodel.h"

#include <QTime>

#include "headers/database/databasemanager.h"

namespace ISXMail
{

EmailListModel::EmailListModel(QObject* parent) :
	QAbstractListModel(parent),
	m_database(ISXDatabaseManager::DatabaseManager::DatabasePath()),
	m_message_repository(m_database),
	m_recipient_repository(m_database)
{
	LoadFromDatabase();
}

int EmailListModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid())
	{
		return 0;
	}

	return static_cast<int>(m_data.size());
}

QVariant EmailListModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
	{
		return {};
	}

	if (index.row() >= static_cast<int>(m_data.size()))
	{
		return {};
	}

	const auto& item = m_data[index.row()];

	switch (role)
	{
	case StarredRole:
		return item.is_starred;
	case SentRole:
		return item.is_sent;
	case DraftRole:
		return item.is_draft;
	case ThemeRole:
		return item.theme;
	case NameRole:
		return item.name;
	case SendToRole:
		return item.send_to;
	case PreviewRole:
		return item.preview;
	case ContentRole:
		return item.content;
	case TimeRole:
		return item.time;
	}

	return {};
}

QHash<int, QByteArray> EmailListModel::roleNames() const
{
	return {
		{StarredRole, "emailsStarred"},
		{SentRole, "emailsSent"},
		{DraftRole, "emailsDraft"},
		{ThemeRole, "emailsTheme"},
		{NameRole, "emailsName"},
		{SendToRole, "emailsSendTo"},
		{PreviewRole, "emailsPreview"},
		{ContentRole, "emailsContent"},
		{TimeRole, "emailsTime"}
	};
}

void EmailListModel::AddData(
	bool is_starred,
	bool is_sent,
	bool is_draft,
	const QString& theme,
	const QString& name,
	const QString& send_to,
	const QString& content,
	const QString& time
)
{
	const QString t = time.isEmpty() ? QTime::currentTime().toString("hh:mm") : time;
	const QString preview = makePreview(content, 30);
	AddData({is_starred, is_sent, is_draft, theme, name, send_to, preview, content, t});
}

QString EmailListModel::makePreview(const QString& text, int maxLen)
{
	if (text.length() <= maxLen)
	{
		return text;
	}

	int pos = text.lastIndexOf(' ', maxLen);

	if (pos < 0)
	{
		pos = maxLen;
	}

	return text.left(pos) + "...";
}

void EmailListModel::AddData(const EmailData& item)
{
	beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
	m_data.push_back(item);
	endInsertRows();
	emit dataAdded();
}

void EmailListModel::LoadFromDatabase()
{
	const auto messages = m_message_repository.FindAll();

	if (messages.empty())
	{
		return;
	}

	beginInsertRows(QModelIndex(), 0, static_cast<int>(messages.size()) - 1);

	for (const auto& message : messages)
	{
		QString recipient_email;
		const auto recipients = m_recipient_repository.FindByMessageId(message.id);

		if (!recipients.empty())
		{
			recipient_email = QString::fromStdString(recipients.front().recipient_email);
		}

		const bool is_draft = message.status == Storage::MailMessageStatus::Draft;
		const bool is_sent = message.status == Storage::MailMessageStatus::Sent;
		const bool is_starred = false;
		const QString theme = message.subject.has_value()
			? QString::fromStdString(*message.subject)
			: QStringLiteral("empty");
		const QString name = QString::fromStdString(message.sender_email);
		const QString content = QString::fromStdString(message.body);
		const QString preview = makePreview(content, 30);
		const QString time = QString::fromStdString(message.created_at);

		m_data.push_back({
			is_starred,
			is_sent,
			is_draft,
			theme,
			name,
			recipient_email,
			preview,
			content,
			time
		});
	}

	endInsertRows();
}

}
