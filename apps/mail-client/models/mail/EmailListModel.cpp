#include "headers/mail/EmailListModel.h"

#include <QTime>

#include <optional>

#include <boost/json.hpp>

#include "headers/database/DatabaseManager.h"
#include "headers/service/Service.h"
#include "headers/users/CurrentUser.h"

namespace json = boost::json;

namespace ISXMail {

    namespace {
        QString GetEnumString(int role)

        {

            switch (role)

            {
            case InboxRole:
                return QStringLiteral("InboxRole");

            case StarredRole:
                return QStringLiteral("StarredRole");

            case SentRole:
                return QStringLiteral("SentRole");

            case DraftRole:
                return QStringLiteral("DraftRole");

            case ThemeRole:
                return QStringLiteral("ThemeRole");

            case NameRole:
                return QStringLiteral("NameRole");

            case PreviewRole:
                return QStringLiteral("PreviewRole");

            case ContentRole:
                return QStringLiteral("ContentRole");

            case TimeRole:
                return QStringLiteral("TimeRole");

            case SendToRole:
                return QStringLiteral("SendToRole");

            case ArchiveRole:
                return QStringLiteral("ArchiveRole");

            default:
                return QStringLiteral("UnknownRole");
            }
        }

        std::string GetStdString(const QString& str)
        {
            return str.toStdString();
        }

        QString NormalizeLineEndings(QString text)
        {
            text.replace(QStringLiteral("\r\n"), QStringLiteral("\n"));
            text.replace(QLatin1Char('\r'), QLatin1Char('\n'));
            return text;
        }

        QString HeaderValue(const QString& raw_text, const QString& header_name)
        {
            const QString text = NormalizeLineEndings(raw_text);
            const QString prefix = header_name + QStringLiteral(":");
            bool reading_header = false;
            QString value;

            for (const QString& line : text.split(QLatin1Char('\n'))) {
                if (line.trimmed().isEmpty()) {
                    break;
                }

                if (reading_header && (line.startsWith(QLatin1Char(' ')) || line.startsWith(QLatin1Char('\t')))) {
                    value += QLatin1Char(' ') + line.trimmed();
                    continue;
                }

                reading_header = line.startsWith(prefix, Qt::CaseInsensitive);
                if (reading_header) {
                    value = line.mid(prefix.length()).trimmed();
                }
            }

            return value;
        }

        QString StripMessageHeaders(const QString& raw_text)
        {
            const QString text = NormalizeLineEndings(raw_text);
            const qsizetype header_end = text.indexOf(QStringLiteral("\n\n"));
            if (header_end < 0) {
                return text.trimmed();
            }

            return text.mid(header_end + 2).trimmed();
        }

        QString DisplaySubject(const QString& subject, const QString& body)
        {
            QString display_subject = subject.trimmed();
            if (display_subject.isEmpty()) {
                display_subject = HeaderValue(body, QStringLiteral("Subject"));
            }

            return display_subject.isEmpty() ? QStringLiteral("(No subject)") : display_subject;
        }

        bool IsServerBacked(const EmailData& item)
        {
            return item.id >= 0 && !item.is_draft;
        }

        bool IsRecipientActor(Storage::MailMessageActorType actor_type)
        {
            return actor_type == Storage::MailMessageActorType::To || actor_type == Storage::MailMessageActorType::Cc ||
                   actor_type == Storage::MailMessageActorType::Bcc;
        }
    } // namespace

    EmailListModel::EmailListModel(QObject* parent)
        : QAbstractListModel(parent)
        , m_database(ISXDatabaseManager::DatabaseManager::DatabasePath())
        , m_message_repository(m_database)
        , m_actor_repository(m_database)
    {
        ISXService::Service::Logger().Log(Logging::LogLevel::Debug, "EmailListModel: constructed");
    }

    int EmailListModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid()) {
            return 0;
        }

        return static_cast<int>(m_data.size());
    }

    QVariant EmailListModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid()) {
            return {};
        }

        if (index.row() >= static_cast<int>(m_data.size())) {
            return {};
        }

        const auto& item = m_data[index.row()];

        switch (role) {
        case InboxRole:
            return item.is_inbox;
        case StarredRole:
            return item.is_starred;
        case SentRole:
            return item.is_sent;
        case DraftRole:
            return item.is_draft;
        case ArchiveRole:
            return item.is_archive;
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
        return {{InboxRole, "emailsInbox"},
                {StarredRole, "emailsStarred"},
                {SentRole, "emailsSent"},
                {DraftRole, "emailsDraft"},
                {ArchiveRole, "emailsArchive"},
                {ThemeRole, "emailsTheme"},
                {NameRole, "emailsName"},
                {SendToRole, "emailsSendTo"},
                {PreviewRole, "emailsPreview"},
                {ContentRole, "emailsContent"},
                {TimeRole, "emailsTime"}};
    }

    void EmailListModel::RemoveData(int row)
    {
        DeleteEmail(row);
    }

    bool EmailListModel::DeleteEmail(int row)
    {
        if (row < 0 || row >= static_cast<int>(m_data.size())) {
            return false;
        }

        if (!DeleteFromStorage(m_data[row])) {
            return false;
        }

        beginRemoveRows(QModelIndex(), row, row);
        m_data.erase(m_data.begin() + row);
        endRemoveRows();
        ISXService::Service::Logger().Log(
            Logging::LogLevel::Debug,
            GetStdString(QString("EmailListModel::DeleteEmail: data was removed at %1").arg(QString::number(row))));
        return true;
    }

    void EmailListModel::AddData(bool is_starred,
                                 bool is_sent,
                                 bool is_draft,
                                 bool is_archive,
                                 const QString& theme,
                                 const QString& name,
                                 const QString& send_to,
                                 const QString& content,
                                 const QString& time,
                                 bool is_inbox)
    {
        const QString t = time.isEmpty() ? QTime::currentTime().toString("hh:mm") : time;
        const QString preview = MakePreview(content, 30);
        AddData({-1, is_inbox, is_starred, is_sent, is_draft, is_archive, theme, name, send_to, preview, content, t});
    }

    QString EmailListModel::MakePreview(const QString& text, int maxLen)
    {
        if (text.length() <= maxLen) {
            return text;
        }

        int pos = text.lastIndexOf(' ', maxLen);

        if (pos < 0) {
            pos = maxLen;
        }

        return text.left(pos) + "...";
    }

    void EmailListModel::AddData(const EmailData& item)
    {
        beginInsertRows(QModelIndex(), 0, 0);
        m_data.insert(m_data.begin(), item);
        endInsertRows();
        ISXService::Service::Logger().Log(Logging::LogLevel::Debug, "EmailListModel::AddData: data was added");
        emit dataAdded();
    }

    bool EmailListModel::SetStarred(int row, bool starred)
    {
        if (row < 0 || row >= static_cast<int>(m_data.size()))
            return false;

        if (!SetStarredInStorage(m_data[row], starred))
            return false;

        m_data[row].is_starred = starred;
        const QModelIndex idx = index(row, 0);
        emit dataChanged(idx, idx, {StarredRole});
        ISXService::Service::Logger().Log(
            Logging::LogLevel::Debug,
            GetStdString(QString("EmailListModel::SetStarred: data at %1 changed %2 field to %3")
                             .arg(row)
                             .arg(GetEnumString(StarredRole))
                             .arg(starred ? "true" : "false")));
        return true;
    }

    bool EmailListModel::RefreshFromServer()
    {
        const QString current_email = ISXCurrentUser::CurrentUser::GetInstance().email();
        if (current_email.trimmed().isEmpty()) {
            return false;
        }

        try {
            const auto response = ISXService::Service::MailServerClient().GetMails(current_email.toStdString());
            if (!response.is_success() || !response.body.is_object()) {
                return false;
            }

            const json::object& object = response.body.as_object();
            const json::value* mails_value = object.if_contains("mails");
            if (mails_value == nullptr || !mails_value->is_array()) {
                return false;
            }

            std::vector<EmailData> server_data;
            for (const json::value& mail_value : mails_value->as_array()) {
                if (!mail_value.is_object()) {
                    continue;
                }

                const json::object& mail = mail_value.as_object();
                const std::string from = mail.contains("from") && mail.at("from").is_string()
                                             ? std::string(mail.at("from").as_string().c_str())
                                             : "";
                const std::string subject = mail.contains("subject") && mail.at("subject").is_string()
                                                ? std::string(mail.at("subject").as_string().c_str())
                                                : "";
                const std::string body = mail.contains("body") && mail.at("body").is_string()
                                             ? std::string(mail.at("body").as_string().c_str())
                                             : "";
                const std::string created_at = mail.contains("created_at") && mail.at("created_at").is_string()
                                                   ? std::string(mail.at("created_at").as_string().c_str())
                                                   : "";
                const std::string status = mail.contains("status") && mail.at("status").is_string()
                                               ? std::string(mail.at("status").as_string().c_str())
                                               : "";

                QString recipient_email;
                bool is_recipient = false;
                const json::value* to_value = mail.if_contains("to");
                if (to_value != nullptr && to_value->is_array()) {
                    for (const json::value& recipient_value : to_value->as_array()) {
                        if (!recipient_value.is_string()) {
                            continue;
                        }

                        const QString recipient =
                            QString::fromStdString(std::string(recipient_value.as_string().c_str()));
                        if (recipient_email.isEmpty()) {
                            recipient_email = recipient;
                        }

                        if (recipient.compare(current_email, Qt::CaseInsensitive) == 0) {
                            is_recipient = true;
                        }
                    }
                }

                const QString sender_email = QString::fromStdString(from);
                const bool is_current_sender = sender_email.compare(current_email, Qt::CaseInsensitive) == 0;
                if (!is_current_sender && !is_recipient) {
                    continue;
                }

                const QString raw_content = QString::fromStdString(body);
                const QString content = StripMessageHeaders(raw_content);
                const QString display_subject = DisplaySubject(QString::fromStdString(subject), raw_content);
                const bool is_draft = mail.contains("is_draft") && mail.at("is_draft").is_bool()
                                          ? mail.at("is_draft").as_bool()
                                          : status == "draft";
                const bool is_archive = mail.contains("is_archive") && mail.at("is_archive").is_bool()
                                            ? mail.at("is_archive").as_bool()
                                            : false;
                const bool is_sent = mail.contains("is_sent") && mail.at("is_sent").is_bool()
                                         ? mail.at("is_sent").as_bool()
                                         : is_current_sender && !is_draft && !is_archive;
                const bool is_inbox = mail.contains("is_inbox") && mail.at("is_inbox").is_bool()
                                          ? mail.at("is_inbox").as_bool()
                                          : is_recipient && !is_current_sender && !is_draft && !is_archive;
                const std::int64_t id = mail.contains("id") && mail.at("id").is_int64() ? mail.at("id").as_int64() : -1;
                const bool is_starred = mail.contains("is_starred") && mail.at("is_starred").is_bool()
                                            ? mail.at("is_starred").as_bool()
                                            : false;

                server_data.push_back({id,
                                       is_inbox,
                                       is_starred,
                                       is_sent,
                                       is_draft,
                                       is_archive,
                                       display_subject,
                                       sender_email,
                                       recipient_email,
                                       MakePreview(content, 30),
                                       content,
                                       QString::fromStdString(created_at)});
            }

            ReplaceData(std::move(server_data));
            return true;
        } catch (const std::exception& exception) {
            ISXService::Service::Logger().Log(
                Logging::LogLevel::Error, std::string("EmailListModel::RefreshFromServer failed: ") + exception.what());
            return false;
        }
    }

    void EmailListModel::ReplaceData(std::vector<EmailData> data)
    {
        beginResetModel();
        m_data = std::move(data);
        endResetModel();
        emit dataAdded();
    }

    void EmailListModel::LoadFromDatabase()
    {
        const QString current_email = ISXCurrentUser::CurrentUser::GetInstance().email();
        if (current_email.trimmed().isEmpty()) {
            return;
        }

        const auto messages = m_message_repository.FindAll();

        if (messages.empty()) {
            return;
        }

        std::vector<EmailData> local_data;

        for (const auto& message : messages) {
            QString sender_email;
            QString recipient_email;
            std::optional<Storage::MailMessageActorRecord> current_actor;
            const auto actors = m_actor_repository.FindByMessageId(message.id);

            for (const auto& actor : actors) {
                const QString actor_email = QString::fromStdString(actor.actor_email);

                if (actor.actor_type == Storage::MailMessageActorType::From) {
                    sender_email = actor_email;
                } else if (IsRecipientActor(actor.actor_type) && recipient_email.isEmpty()) {
                    recipient_email = actor_email;
                }

                if (!actor.deleted_at.has_value() && actor_email.compare(current_email, Qt::CaseInsensitive) == 0 &&
                    !current_actor.has_value()) {
                    current_actor = actor;
                }
            }

            if (!current_actor.has_value()) {
                continue;
            }

            const QString raw_content = QString::fromStdString(message.body);
            const QString theme = DisplaySubject(
                message.subject.has_value() ? QString::fromStdString(*message.subject) : QString(), raw_content);
            const QString content = StripMessageHeaders(raw_content);
            const QString preview = MakePreview(content, 30);
            const QString time = QString::fromStdString(message.created_at);
            const bool is_draft = message.message_status == Storage::MailMessageStatus::Draft;
            const bool is_archive = current_actor->archived_at.has_value();
            const bool is_current_sender = current_actor->actor_type == Storage::MailMessageActorType::From;
            const bool is_sent = is_current_sender && !is_draft && !is_archive;
            const bool is_inbox = !is_current_sender && !is_draft && !is_archive;

            local_data.push_back({message.id,
                                  is_inbox,
                                  current_actor->starred_at.has_value(),
                                  is_sent,
                                  is_draft,
                                  is_archive,
                                  theme,
                                  sender_email,
                                  recipient_email,
                                  preview,
                                  content,
                                  time});
        }

        if (local_data.empty()) {
            return;
        }

        beginInsertRows(QModelIndex(), 0, static_cast<int>(local_data.size()) - 1);
        m_data.insert(m_data.end(), local_data.begin(), local_data.end());
        endInsertRows();
        ISXService::Service::Logger().Log(Logging::LogLevel::Debug,
                                          "EmailListModel::LoadFromDatabase: data was loaded from database");
    }

    bool EmailListModel::setData(const QModelIndex& index, const QVariant& value, int role)
    {
        if (!index.isValid() || index.row() >= static_cast<int>(m_data.size())) {
            return false;
        }
        switch (role) {
        case StarredRole:
            return SetStarred(index.row(), value.toBool());
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

        ISXService::Service::Logger().Log(
            Logging::LogLevel::Debug,
            GetStdString(QString("EmailListModel::setData: data at %1 changed value of role %2")
                             .arg(QString::number(index.row()))
                             .arg(GetEnumString(role))));
        emit dataChanged(index, index, {role});
        return true;
    }

    bool EmailListModel::ToggleArchive(int row)
    {
        if (row < 0 || row >= static_cast<int>(m_data.size())) {
            return false;
        }

        const bool archived = !m_data[row].is_archive;
        if (!SetArchivedInStorage(m_data[row], archived)) {
            return false;
        }

        m_data[row].is_archive = archived;
        const QModelIndex idx = index(row, 0);
        emit dataChanged(idx, idx, {ArchiveRole});

        Logging::Logger::Instance().Log(
            Logging::LogLevel::Debug,
            GetStdString(QString("EmailListModel::ToggleArchive: data at %1 changed %2 field to %3")
                             .arg(row)
                             .arg(GetEnumString(ArchiveRole))
                             .arg(m_data[row].is_archive ? "true" : "false")));

        return m_data[row].is_archive;
    }

    bool EmailListModel::DeleteFromStorage(const EmailData& item)
    {
        if (item.id < 0) {
            return true;
        }

        if (!IsServerBacked(item)) {
            return m_message_repository.DeleteMessage(item.id);
        }

        try {
            const QString current_email = ISXCurrentUser::CurrentUser::GetInstance().email();
            if (current_email.trimmed().isEmpty()) {
                return false;
            }

            const auto response =
                ISXService::Service::MailServerClient().DeleteMail(item.id, current_email.toStdString());
            return response.is_success();
        } catch (const std::exception& exception) {
            ISXService::Service::Logger().Log(
                Logging::LogLevel::Error, std::string("EmailListModel::DeleteFromStorage failed: ") + exception.what());
            return false;
        }
    }

    bool EmailListModel::SetStarredInStorage(const EmailData& item, bool starred)
    {
        if (item.id < 0) {
            return true;
        }

        const QString current_email = ISXCurrentUser::CurrentUser::GetInstance().email();
        if (current_email.trimmed().isEmpty()) {
            return false;
        }

        if (!IsServerBacked(item)) {
            return m_actor_repository.SetStarred(item.id, current_email.toStdString(), starred);
        }

        try {
            const std::string user_email = current_email.toStdString();
            const auto response = starred ? ISXService::Service::MailServerClient().StarMail(item.id, user_email)
                                          : ISXService::Service::MailServerClient().UnstarMail(item.id, user_email);
            return response.is_success();
        } catch (const std::exception& exception) {
            ISXService::Service::Logger().Log(Logging::LogLevel::Error,
                                              std::string("EmailListModel::SetStarredInStorage failed: ") +
                                                  exception.what());
            return false;
        }
    }

    bool EmailListModel::SetArchivedInStorage(const EmailData& item, bool archived)
    {
        if (item.id < 0) {
            return true;
        }

        const QString current_email = ISXCurrentUser::CurrentUser::GetInstance().email();
        if (current_email.trimmed().isEmpty()) {
            return false;
        }

        if (!IsServerBacked(item)) {
            return m_actor_repository.SetArchived(item.id, current_email.toStdString(), archived);
        }

        try {
            const std::string user_email = current_email.toStdString();
            const auto response = archived ? ISXService::Service::MailServerClient().ArchiveMail(item.id, user_email)
                                           : ISXService::Service::MailServerClient().UnarchiveMail(item.id, user_email);
            return response.is_success();
        } catch (const std::exception& exception) {
            ISXService::Service::Logger().Log(Logging::LogLevel::Error,
                                              std::string("EmailListModel::SetArchivedInStorage failed: ") +
                                                  exception.what());
            return false;
        }
    }

    Qt::ItemFlags EmailListModel::flags(const QModelIndex& index) const
    {
        return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
    }

} // namespace ISXMail
