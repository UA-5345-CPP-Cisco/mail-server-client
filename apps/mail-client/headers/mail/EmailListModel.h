#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QtMath>

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "mail_storage/Database.h"
#include "mail_storage/MailMessageRepository.h"
#include "mail_storage/MessageRecipientRepository.h"

namespace ISXMail {

    Q_NAMESPACE;

    struct EmailData
    {
        std::int64_t id{-1};
        bool is_inbox;
        bool is_starred;
        bool is_sent;
        bool is_draft;
        bool is_archive;
        bool is_seen;
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
        ArchiveRole,
        SeenRole,
        InboxRole,
        ThemeRole,
        NameRole,
        SendToRole,
        PreviewRole,
        ContentRole,
        TimeRole
    };

    Q_ENUM_NS(EmailRole);

    class EmailListModel : public QAbstractListModel
    {
        Q_OBJECT;
        Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
        Q_PROPERTY(bool serverError READ serverError NOTIFY serverErrorChanged)
        using InboxMessageCallback = std::function<void(const QString& sender, const QString& subject, const QString& preview)>;
    public:
        explicit EmailListModel(QObject* parent = nullptr);

        bool isLoading() const;
        bool serverError() const;

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;

        void RemoveData(int row);
        Q_INVOKABLE bool DeleteEmail(int row);
        Q_INVOKABLE void AddData(bool is_starred,
                                 bool is_sent,
                                 bool is_draft,
                                 bool is_archive,
                                 bool is_seen,
                                 const QString& theme,
                                 const QString& name,
                                 const QString& send_to,
                                 const QString& content,
                                 const QString& time,
                                 bool is_inbox = false);
        using InboxMessageCallback =
            std::function<void(const QString& sender, const QString& subject, const QString& preview)>;
        void registerInboxMessageCallback(InboxMessageCallback callback);
        Q_INVOKABLE bool SetStarred(int row, bool starred);
        bool ToggleArchive(int row);
        void AddData(const EmailData& item);
        bool UpdateSeen(int row, bool seen);
        Q_INVOKABLE bool RefreshFromServer(bool silent = false);

        Q_INVOKABLE bool RefreshFromServer();

        void registerInboxMessageCallback(InboxMessageCallback callback);
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

    signals:
        void dataAdded();
        void inboxMessageReceived(const QString& sender, const QString& subject, const QString& preview);
        void isLoadingChanged();
        void serverErrorChanged();

    private:
        void LoadFromDatabase();
        void ReplaceData(std::vector<EmailData> data);
        QString MakePreview(const QString& text, int maxLen = 50);
        QString DefaultDatabasePath() const;

        Storage::Database m_database;
        Storage::MailMessageRepository m_message_repository;
        Storage::MessageRecipientRepository m_recipient_repository;
        std::vector<EmailData> m_data;
        std::vector<InboxMessageCallback> m_inbox_callbacks;
        bool m_isLoading{false};
        bool m_serverError{false};
        bool m_isFirstSync{true};
        QString m_lastFetchedEmail;
    };

} // namespace ISXMail