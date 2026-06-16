#include "headers/mail/emaillistmodel.h"
#include <QTime>

namespace ISXMail
{

EmailListModel::EmailListModel(QObject* parent)
    : QAbstractListModel(parent)
{
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
        case StarredRole: return item.is_starred;
        case SentRole: return item.is_sent;
        case DraftRole: return item.is_draft;
        case ThemeRole: return item.theme;
        case NameRole: return item.name;
        case SendToRole: return item.send_to;
        case PreviewRole: return item.preview;
        case ContentRole: return item.content;
        case TimeRole: return item.time;
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

void EmailListModel::AddData(bool is_starred, bool is_sent, bool is_draft,
                             const QString& theme, const QString& name,
                             const QString& send_to, const QString& content, const QString& time)
{
    QString t = time.isEmpty() ? QTime::currentTime().toString("hh:mm") : time;
    QString preview = makePreview(content, 30);
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

} // namespace ISXMail