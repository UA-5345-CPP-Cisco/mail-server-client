#include "headers/users/AccountListModel.h"

#include "headers/database/DatabaseManager.h"
#include "../../../../libs/mail-storage/include/mail_storage/UserRepository.h"
#include "headers/users/CurrentUser.h"

namespace ISXMail
{
AccountListModel::AccountListModel(QObject* parent) : QAbstractListModel(parent), m_database(ISXDatabaseManager::DatabaseManager::DatabasePath())
{
    LoadFromDatabase();
}

int AccountListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_data.size());
}

QVariant AccountListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 ||
        static_cast<size_t>(index.row()) >= m_data.size())
        return {};

    const AccountData& item = m_data[index.row()];

    switch (role)
    {
        case AccountNameRole:    return item.account_name;
        case AccountEmailRole:   return item.account_email;
        case AvatarUrlRole:      return item.avatar_url;
        case AvatarColorRole:    return item.avatar_color;
        case AvatarInitialRole:  return item.avatar_initial;
        case IsActiveRole:       return item.is_active;
        default:                 return {};
    }
}

QHash<int, QByteArray> AccountListModel::roleNames() const
{
    return
    {
        { AccountNameRole,   "accountName" },
        { AccountEmailRole,  "accountEmail" },
        { AvatarUrlRole,     "avatarUrl" },
        { AvatarColorRole,   "avatarColor" },
        { AvatarInitialRole, "avatarInitial" },
        { IsActiveRole,      "isActive" }
    };
}

bool AccountListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() < 0 ||
        static_cast<size_t>(index.row()) >= m_data.size())
        return false;

    AccountData& item = m_data[index.row()];

    switch (role)
    {
        case AccountNameRole:    item.account_name = value.toString(); break;
        case AccountEmailRole:   item.account_email = value.toString(); break;
        case AvatarUrlRole:      item.avatar_url = value.toString(); break;
        case AvatarColorRole:    item.avatar_color = value.toString(); break;
        case AvatarInitialRole:  item.avatar_initial = value.toString(); break;
        case IsActiveRole:       item.is_active = value.toBool(); break;
        default:                 return false;
    }

    emit dataChanged(index, index, { role });
    return true;
}

void AccountListModel::AddData(const AccountData& item)
{
    const int row = static_cast<int>(m_data.size());
    beginInsertRows(QModelIndex(), row, row);
    m_data.push_back(item);
    endInsertRows();
    emit accountAdded();
}

void AccountListModel::AddAccount(const QString& name, const QString& email,
                                   const QString& avatarUrl, const QString& avatarColor,
                                   const QString& avatarInitial, bool isActive)
{
    AccountData item;
    item.account_name = name;
    item.account_email = email;
    item.avatar_url = avatarUrl;
    item.avatar_color = avatarColor;
    item.avatar_initial = avatarInitial;
    item.is_active = isActive;

    AddData(item);

    if (isActive)
        SetActiveAccount(static_cast<int>(m_data.size())- 1);
}

bool AccountListModel::RemoveAccount(int row)
{
    if (row < 0 || static_cast<size_t>(row) >= m_data.size())
    {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_data.erase(m_data.begin() + row);
    endRemoveRows();
    return true;
}

bool AccountListModel::SetActiveAccount(int row)
{
    if (row < 0 || static_cast<size_t>(row) >= m_data.size())
    {
        return false;
    }

    Storage::UserRepository repo(m_database);

    for (size_t i = 0; i < m_data.size(); ++i)
    {
        const bool should_be_active = (static_cast<int>(i) == row);

        if (m_data[i].is_active != should_be_active)
        {
            m_data[i].is_active = should_be_active;

            std::string std_email = m_data[i].account_email.toStdString();
            auto user_record = repo.FindByEmail(std_email);

            if (user_record.has_value())
            {
                auto db_status = should_be_active ? Storage::UserStatus::Active : Storage::UserStatus::Disabled;
                repo.UpdateStatus(user_record->id, db_status);
            }

            // Synchronize the global application context with the newly activated user
            if (should_be_active)
            {
                ISXCurrentUser::CurrentUser::GetInstance().Authorize(
                    m_data[i].account_name,
                    m_data[i].account_email,
                    m_data[i].avatar_url
                    );
            }

            // Notify Qt views that the active role has changed to trigger a UI repaint
            const QModelIndex idx = index(static_cast<int>(i));
            emit dataChanged(idx, idx, { IsActiveRole });
        }
    }

    emit activeAccountChanged(row);
    return true;
}

int AccountListModel::ActiveAccountRow() const
{
    for (size_t i = 0; i < m_data.size(); ++i)
        if (m_data[i].is_active)
            return static_cast<int>(i);
    return -1;
}


void AccountListModel::LoadFromDatabase()
{
    m_data.clear();

    Storage::UserRepository user_repo(m_database);
    std::vector<Storage::UserRecord> db_users = user_repo.FindAll();

    std::vector<AccountData> loaded_accounts;
    int saved_active_row = -1;
    int counter = 0;

    for (const auto& user : db_users)
    {
        AccountData account;
        account.account_name = QString::fromStdString(user.username);
        account.account_email = QString::fromStdString(user.email);
        account.avatar_url = "";
        account.avatar_color = "#3b82f6";

        account.avatar_initial = account.account_name.isEmpty() ? '?' : account.account_name.at(0).toUpper();
        account.is_active = (user.status == Storage::UserStatus::Active);

        if (account.is_active)
        {
            saved_active_row = static_cast<int>(loaded_accounts.size());
        }

        loaded_accounts.push_back(account);
    }

    if (!loaded_accounts.empty())
    {
        beginInsertRows(QModelIndex(), 0, static_cast<int>(loaded_accounts.size()) - 1);
        m_data = std::move(loaded_accounts);
        endInsertRows();

        if (saved_active_row != -1)
        {
            SetActiveAccount(saved_active_row);
        }
        else
        {
            SetActiveAccount(0);
        }
    }
}

QString AccountListModel::DefaultDatabasePath() const
{
    return QString();
}

} // namespace ISXMail
