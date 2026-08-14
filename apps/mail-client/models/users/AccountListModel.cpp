#include "users/AccountListModel.h"

#include <QSettings>

#include "service/Service.h"
#include "users/CurrentUser.h"

namespace ISXMail {

    namespace {
        QString GetEnumString(int role)
        {
            switch (role) {
            case AccountNameRole:
                return QStringLiteral("AccountNameRole");
            case AccountEmailRole:
                return QStringLiteral("AccountEmailRole");
            case AvatarUrlRole:
                return QStringLiteral("AvatarUrlRole");
            case AvatarColorRole:
                return QStringLiteral("AvatarColorRole");
            case AvatarInitialRole:
                return QStringLiteral("AvatarInitialRole");
            case IsActiveRole:
                return QStringLiteral("AvatarInitialRole");
            default:
                return QStringLiteral("UnknownRole");
            }
        }

        std::string GetStdString(const QString& str)
        {
            return str.toStdString();
        }
    } // namespace

    AccountListModel::AccountListModel(QObject* parent)
        : QAbstractListModel(parent)
    {
        LoadFromSettings();
        ISXService::Service::Logger().Log(Logging::LogLevel::Debug, "AccountListModel: constructed");
    }

    int AccountListModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(m_data.size());
    }

    QVariant AccountListModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || index.row() < 0 || static_cast<size_t>(index.row()) >= m_data.size())
            return {};

        const AccountData& item = m_data[index.row()];

        switch (role) {
        case AccountNameRole:
            return item.account_name;
        case AccountEmailRole:
            return item.account_email;
        case AvatarUrlRole:
            return item.avatar_url;
        case AvatarColorRole:
            return item.avatar_color;
        case AvatarInitialRole:
            return item.avatar_initial;
        case IsActiveRole:
            return item.is_active;
        default:
            return {};
        }
    }

    QHash<int, QByteArray> AccountListModel::roleNames() const
    {
        return {{AccountNameRole, "accountName"},
                {AccountEmailRole, "accountEmail"},
                {AvatarUrlRole, "avatarUrl"},
                {AvatarColorRole, "avatarColor"},
                {AvatarInitialRole, "avatarInitial"},
                {IsActiveRole, "isActive"}};
    }

    bool AccountListModel::setData(const QModelIndex& index, const QVariant& value, int role)
    {
        if (!index.isValid() || index.row() < 0 || static_cast<size_t>(index.row()) >= m_data.size())
            return false;

        AccountData& item = m_data[index.row()];

        switch (role) {
        case AccountNameRole:
            item.account_name = value.toString();
            break;
        case AccountEmailRole:
            item.account_email = value.toString();
            break;
        case AvatarUrlRole:
            item.avatar_url = value.toString();
            break;
        case AvatarColorRole:
            item.avatar_color = value.toString();
            break;
        case AvatarInitialRole:
            item.avatar_initial = value.toString();
            break;
        case IsActiveRole:
            item.is_active = value.toBool();
            break;
        default:
            return false;
        }

        ISXService::Service::Logger().Log(
            Logging::LogLevel::Debug,
            GetStdString(QString("AccountListModel::setData: data at %1 changed value of role %2")
                             .arg(QString::number(index.row()))
                             .arg(GetEnumString(role))));
        emit dataChanged(index, index, {role});
        return true;
    }

    void AccountListModel::AddData(const AccountData& item)
    {
        const int row = static_cast<int>(m_data.size());
        beginInsertRows(QModelIndex(), row, row);
        m_data.push_back(item);
        endInsertRows();
        emit accountAdded();

        ISXService::Service::Logger().Log(Logging::LogLevel::Debug,
                                          GetStdString("AccountListModel::AddData: data was added"));
    }

    void AccountListModel::AddAccount(const QString& name,
                                      const QString& email,
                                      const QString& avatarUrl,
                                      const QString& avatarColor,
                                      const QString& avatarInitial,
                                      bool isActive)
    {
        AccountData item;
        item.account_name = name;
        item.account_email = email;
        item.avatar_url = avatarUrl;
        item.avatar_color = avatarColor;
        item.avatar_initial = avatarInitial;
        item.is_active = false;

        AddData(item);

        if (isActive)
            SetActiveAccount(static_cast<int>(m_data.size()) - 1);

        SaveToSettings();
    }

    bool AccountListModel::RemoveAccount(int row)
    {
        if (row < 0 || static_cast<size_t>(row) >= m_data.size()) {
            return false;
        }

        bool was_active = m_data[row].is_active;
        QString email_to_remove = m_data[row].account_email;

        beginRemoveRows(QModelIndex(), row, row);
        m_data.erase(m_data.begin() + row);
        endRemoveRows();

        ISXService::Service::Logger().Log(
            Logging::LogLevel::Debug,
            GetStdString("AccountListModel::RemoveAccount: data was removed at " + QString::number(row)));
        SaveToSettings();

        if (was_active)
        {
            if (!m_data.empty())
            {
                SetActiveAccount(0);
            }
            else
            {
                emit activeAccountChanged(-1);
                ISXCurrentUser::CurrentUser::GetInstance().Logout();
                ISXCurrentUser::CurrentUser::GetInstance().NotifyNoAccountsLeft();
            }
        }

        ISXService::Service::Logger().Log(Logging::LogLevel::Info, "Local account removed from QSettings: " + email_to_remove.toStdString());

        return true;
    }

    bool AccountListModel::SetActiveAccount(int row)
    {
        if (row < 0 || static_cast<size_t>(row) >= m_data.size()) {
            return false;
        }

        for (size_t i = 0; i < m_data.size(); ++i) {
            const bool should_be_active = (static_cast<int>(i) == row);

            if (m_data[i].is_active != should_be_active) {
                m_data[i].is_active = should_be_active;

                const QModelIndex idx = index(static_cast<int>(i));
                emit dataChanged(idx, idx, {IsActiveRole});
            }

            // Synchronize the global application context with the newly activated user
            if (should_be_active) {
                ISXCurrentUser::CurrentUser::GetInstance().Authorize(
                    m_data[i].account_name, m_data[i].account_email, m_data[i].avatar_url);
            }
        }

        SaveToSettings();

        ISXService::Service::Logger().Log(
            Logging::LogLevel::Debug,
            GetStdString("AccountListModel::SetActiveAccount: account is active at " + QString::number(row)));
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

    QString AccountListModel::DefaultDatabasePath() const
    {
        return QString();
    }

    bool AccountListModel::LoadFromSettings()
    {
        QSettings settings("ISX", "MailClient");
        int size = settings.beginReadArray("accounts");
        if (size == 0) {
            settings.endArray();
            ISXService::Service::Logger().Log(
                Logging::LogLevel::Debug, GetStdString("AccountListModel::LoadFromSettings: no accounts in settings"));
            return false;
        }

        std::vector<AccountData> loaded_accounts;
        int active_row = -1;
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            AccountData account;
            account.account_name = settings.value("account_name").toString();
            account.account_email = settings.value("account_email").toString();
            account.avatar_url = settings.value("avatar_url").toString();
            account.avatar_color = settings.value("avatar_color").toString();
            account.avatar_initial = settings.value("avatar_initial").toString();
            account.is_active = settings.value("is_active").toBool();

            if (account.is_active) {
                active_row = static_cast<int>(loaded_accounts.size());
            }
            loaded_accounts.push_back(account);
        }
        settings.endArray();

        if (!loaded_accounts.empty()) {
            beginInsertRows(QModelIndex(), 0, static_cast<int>(loaded_accounts.size()) - 1);
            m_data = std::move(loaded_accounts);
            endInsertRows();

            if (active_row != -1) {
                SetActiveAccount(active_row);
            } else {
                SetActiveAccount(0);
            }
            ISXService::Service::Logger().Log(
                Logging::LogLevel::Debug,
                GetStdString("AccountListModel::LoadFromSettings: account was loaded successfully!"));
            return true;
        }

        ISXService::Service::Logger().Log(
            Logging::LogLevel::Debug,
            GetStdString("AccountListModel::LoadFromSettings: failed to load account from settings"));
        return false;
    }

    void AccountListModel::SaveToSettings()
    {
        QSettings settings("ISX", "MailClient");
        settings.beginWriteArray("accounts");
        for (int i = 0; i < static_cast<int>(m_data.size()); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("account_name", m_data[i].account_name);
            settings.setValue("account_email", m_data[i].account_email);
            settings.setValue("avatar_url", m_data[i].avatar_url);
            settings.setValue("avatar_color", m_data[i].avatar_color);
            settings.setValue("avatar_initial", m_data[i].avatar_initial);
            settings.setValue("is_active", m_data[i].is_active);
        }
        settings.endArray();
        ISXService::Service::Logger().Log(
            Logging::LogLevel::Debug, GetStdString("AccountListModel::SaveToSettings: data was saved into settings"));
    }

} // namespace ISXMail
