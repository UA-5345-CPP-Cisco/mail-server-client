#include "headers/mail/EmailListModel.h"

#include <QTime>

#include "headers/database/DatabaseManager.h"
#include "logger/Logger.h"

namespace ISXMail
{

namespace
{
  static Logging::Logger kLogger(Logging::LogLevel::Debug, true);
  QString GetEnumString(int role)
  {
    switch (role)
    {
      case InboxRole: return "InboxRole";
      case StarredRole: return "StarredRole";
      case SentRole: return "SentRole";
      case DraftRole: return "DraftRole";
      case ThemeRole: return "ThemeRole";
      case NameRole: return "NameRole";
      case PreviewRole: return "PreviewRole";
      case ContentRole: return "ContentRole";
      case TimeRole: return "TimeRole";
      case SendToRole: return "SendToRole";
    }
  }

  std::string GetStdString(const QString& str)
  {
    return str.toStdString();
  }
}

EmailListModel::EmailListModel(QObject* parent) :
	QAbstractListModel(parent),
	m_database(ISXDatabaseManager::DatabaseManager::DatabasePath()),
	m_message_repository(m_database),
	m_recipient_repository(m_database)
{
  kLogger.Log(Logging::LogLevel::Debug, "EmailListModel: constructed");
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
	case InboxRole:
		return item.is_inbox;
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
		{InboxRole, "emailsInbox"},
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

void EmailListModel::RemoveData(int row)
{
	  if (row < 0 || row >= static_cast<int>(m_data.size()))
	  {
		  return;
	  }
    if (m_data[row].id >= 0 && !m_message_repository.DeleteMessage(m_data[row].id))
    {
	    return;
    }
    beginRemoveRows(QModelIndex(), row, row);
    m_data.erase(m_data.begin() + row);
    endRemoveRows();

    kLogger.Log(Logging::LogLevel::Debug, GetStdString(QString("EmailListModel::RemoveData: data was removed at %1").arg(QString::number(row))));
}

bool EmailListModel::DeleteEmail(int row)
{
	if (row < 0 || row >= static_cast<int>(m_data.size()))
	{
		return false;
	}

	const std::int64_t message_id = m_data[row].id;
	if (message_id >= 0 && !m_message_repository.DeleteMessage(message_id))
	{
		return false;
	}

    beginRemoveRows(QModelIndex(), row, row);
    m_data.erase(m_data.begin() + row);
    endRemoveRows();
	kLogger.Log(Logging::LogLevel::Debug, GetStdString(QString("EmailListModel::RemoveData: data was removed at %1").arg(QString::number(row))));
    return true;
    
}

void EmailListModel::AddData(
	bool is_starred,
	bool is_sent,
	bool is_draft,
	const QString& theme,
	const QString& name,
	const QString& send_to,
	const QString& content,
	const QString& time,
	bool is_inbox
)
{
	const QString t = time.isEmpty() ? QTime::currentTime().toString("hh:mm") : time;
	const QString preview = MakePreview(content, 30);
	AddData({-1, is_inbox, is_starred, is_sent, is_draft, theme, name, send_to, preview, content, t});
}

QString EmailListModel::MakePreview(const QString& text, int maxLen)
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
	beginInsertRows(QModelIndex(), 0, 0);
	m_data.insert(m_data.begin(), item);
	endInsertRows();
  kLogger.Log(Logging::LogLevel::Debug, "EmailListModel::AddData: data was added");
	emit dataAdded();
}

bool EmailListModel::SetStarred(int row, bool starred)
{
    if (row < 0 || row >= static_cast<int>(m_data.size()))
        return false;

    const std::int64_t message_id = m_data[row].id;
    if (message_id >= 0 && !m_message_repository.UpdateStarred(message_id, starred))
        return false;

    m_data[row].is_starred = starred;
    const QModelIndex idx = index(row, 0);
    emit dataChanged(idx, idx, {StarredRole});
    kLogger.Log(Logging::LogLevel::Debug, GetStdString(QString("EmailListModel::SetStarred: data at %1 changed %2 field to %3")
      .arg(row)
      .arg(GetEnumString(StarredRole))
      .arg(starred ? "true" : "false")));
    return true;
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

		const QString theme = message.subject.has_value()
			? QString::fromStdString(*message.subject)
			: QStringLiteral("empty");
		const QString name = QString::fromStdString(message.sender_email);
		const QString content = QString::fromStdString(message.body);
		const QString preview = MakePreview(content, 30);
		const QString time = QString::fromStdString(message.created_at);
		const bool is_inbox = message.is_inbox;
		const bool is_draft = message.is_draft || message.status == Storage::MailMessageStatus::Draft;
		const bool is_sent = !is_inbox && message.status == Storage::MailMessageStatus::Sent;

    m_data.push_back({
			message.id,
			is_inbox,
			message.is_starred,
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
  kLogger.Log(Logging::LogLevel::Debug, "EmailListModel::LoadFromDatabase: data was loaded from database");
}

bool EmailListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_data.size()))
    {
	    return false;
    }
    switch (role)
    {
    case StarredRole:
        if (m_data[index.row()].id >= 0 &&
            !m_message_repository.UpdateStarred(m_data[index.row()].id, value.toBool()))
        {
	        return false;
        }
        m_data[index.row()].is_starred = value.toBool();
        break;
    case ThemeRole:
        m_data[index.row()].theme = value.toString();
        break;
    case SendToRole:
        m_data[index.row()].send_to = value.toString();
        break;
    case ContentRole:
        m_data[index.row()].content = value.toString();
        break;
    default:
        return false;
    }

    kLogger.Log(Logging::LogLevel::Debug, GetStdString(QString("EmailListModel::setData: data at %1 changed value of role %2")
      .arg(QString::number(index.row()))
      .arg(GetEnumString(role))
      ));
    emit dataChanged(index, index, {role});
    return true;
}

Qt::ItemFlags EmailListModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

}
