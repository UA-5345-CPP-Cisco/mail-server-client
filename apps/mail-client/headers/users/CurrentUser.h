#pragma once

#include <QObject>
#include <QString>

namespace ISXCurrentUser
{
class CurrentUser : public QObject
{
    Q_OBJECT

    // PROPERTIES
    Q_PROPERTY(bool isAuthorized READ is_authorized NOTIFY authorizationChanged)
    Q_PROPERTY(QString username READ username NOTIFY profileChanged)
    Q_PROPERTY(QString email READ email NOTIFY profileChanged)
    Q_PROPERTY(QString avatarPath READ avatar_path NOTIFY profileChanged)

public:
    // CONSTRUCTORS
    CurrentUser(const CurrentUser&) = delete;
    CurrentUser& operator=(const CurrentUser&) = delete;

    // METHODS
    static CurrentUser& GetInstance();

    bool is_authorized() const;
    QString username() const;
    QString email() const;
    QString avatar_path() const;
    void SaveAvatarToSqlite(const QString& local_path);
    void SaveAvatarFromSqlite(const QString& local_path);

    Q_INVOKABLE void Authorize(const QString& username, const QString& email, const QString& avatar_path = "");
    Q_INVOKABLE void Logout();
    Q_INVOKABLE void UpdateAvatarPath(const QString& new_path);

signals:
    // SIGNALS
    void authorizationChanged();
    void profileChanged();

private:
    // CONSTRUCTORS
    explicit CurrentUser(QObject* parent = nullptr);
    ~CurrentUser() override = default;

    // DATA MEMBERS
    bool m_is_authorized;
    QString m_username;
    QString m_email;
    QString m_avatar_path;
};

}
