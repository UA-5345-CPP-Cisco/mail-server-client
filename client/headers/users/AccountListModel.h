#ifndef AccountListModel_H
#define AccountListModel_H
#pragma once
#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <vector>
#include "headers/database/Database.h"

namespace ISXMail
{

struct AccountData
{
    QString account_name;
    QString account_email;
    QString avatar_url;
    QString avatar_color;
    QString avatar_initial;
    bool    is_active = false;
};

enum AccountRole
{
    AccountNameRole = Qt::UserRole + 1,
    AccountEmailRole,
    AvatarUrlRole,
    AvatarColorRole,
    AvatarInitialRole,
    IsActiveRole
};

class AccountListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUM(AccountRole)

public:
    explicit AccountListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    void AddData(const AccountData& item);
    Q_INVOKABLE void AddAccount(const QString& name, const QString& email,
                                const QString& avatarUrl, const QString& avatarColor,
                                const QString& avatarInitial, bool isActive = false);

    Q_INVOKABLE bool RemoveAccount(int row);
    Q_INVOKABLE bool SetActiveAccount(int row);
    Q_INVOKABLE int  ActiveAccountRow() const;

signals:
    void accountAdded();
    void activeAccountChanged(int row);

private:
    void LoadFromDatabase();
    QString DefaultDatabasePath() const;

    Storage::Database m_database;
    std::vector<AccountData> m_data;
};

} // namespace ISXMail
#endif // AccountListModel_H
