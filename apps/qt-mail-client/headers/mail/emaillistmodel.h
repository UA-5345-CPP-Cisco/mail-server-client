#pragma once

#include <memory>
#include <cstdint>
#include <vector>

#include <QObject>
#include <QAbstractListModel>
#include <QtMath>
#include <QString>


#include "../../../../libs/storage/include/storage/Database.h"
#include "../../../../libs/storage/include/storage/MailMessageRepository.h"
#include "../../../../libs/storage/include/storage/MessageRecipientRepository.h"

namespace ISXMail
{

Q_NAMESPACE

struct EmailData
{
    std::int64_t id{-1};
    bool is_inbox;
    bool is_starred;
    bool is_sent;
    bool is_draft;
    QString theme;
    QString name;
    QString send_to;
    QString preview;
    QString content;
    QString time;
};

enum EmailRole
{
    StarredRole = Qt::UserRole + 1,
    SentRole,
    DraftRole,
    InboxRole,
    ThemeRole,
    NameRole,
    SendToRole,
    PreviewRole,
    ContentRole,
    TimeRole
};

Q_ENUM_NS(EmailRole)

class EmailListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit EmailListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void RemoveData(int row);
    Q_INVOKABLE bool DeleteEmail(int row);
    Q_INVOKABLE void AddData(bool is_starred, bool is_sent, bool is_draft,
                             const QString& theme, const QString& name,
                             const QString& send_to, const QString& content, const QString& time,
                             bool is_inbox = false);
    Q_INVOKABLE bool SetStarred(int row, bool starred);
    void AddData(const EmailData& item);

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

signals:
    void dataAdded();

private:
    void LoadFromDatabase();
    QString MakePreview(const QString& text, int maxLen = 50);
    QString DefaultDatabasePath() const;

    Storage::Database m_database;
    Storage::MailMessageRepository m_message_repository;
    Storage::MessageRecipientRepository m_recipient_repository;
    std::vector<EmailData> m_data;
};

} // namespace ISXMail
