#include "headers/users/accountlistmodel.h"

namespace ISXMail
{

AccountListModel::AccountListModel(QObject* parent)
    : QAbstractListModel(parent)
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

    if (isActive)
        SetActiveAccount(static_cast<int>(m_data.size())); // станет активным после вставки

    AddData(item);
}

bool AccountListModel::RemoveAccount(int row)
{
    if (row < 0 || static_cast<size_t>(row) >= m_data.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    m_data.erase(m_data.begin() + row);
    endRemoveRows();
    return true;
}

bool AccountListModel::SetActiveAccount(int row)
{
    if (row < 0 || static_cast<size_t>(row) >= m_data.size())
        return false;

    for (size_t i = 0; i < m_data.size(); ++i)
    {
        const bool shouldBeActive = (static_cast<int>(i) == row);
        if (m_data[i].is_active != shouldBeActive)
        {
            m_data[i].is_active = shouldBeActive;
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
    // TODO: подключить реальную загрузку из БД/конфига.
    // Временные данные для отладки UI — убрать после интеграции с реальным хранилищем.
    AddData({ "Personal", "alexm@gmail.com", "", "#2b7fff", "P", true });
    AddData({ "Work",     "alex@company.com", "", "#7c3aed", "W", false });
    AddData({ "Walk",     "alex@no.com", "", "#7c3aed", "G", false });
    AddData({ "Walk",     "alex@no.com", "", "#7c3aed", "G", false });
    AddData({ "Walk",     "alex@no.com", "", "#7c3aed", "G", false });
    AddData({ "Walk",     "alex@no.com", "", "#7c3aed", "G", false });
    AddData({ "Walk",     "alex@no.com", "", "#7c3aed", "G", false });
}

QString AccountListModel::DefaultDatabasePath() const
{
    // TODO: вернуть реальный путь к БД аккаунтов.
    return QString();
}

} // namespace ISXMail
