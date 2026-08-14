#pragma once

#include <QObject>
#include <QString>

class AuthHandler : public QObject
{
    Q_OBJECT
public:
    enum class AuthResult
    {
        Success,
        WrongPasswordOREmail,
        InternalError,
        DatabaseError,
        UserAlreadyExists
    };
    Q_ENUM(AuthResult)
    explicit AuthHandler(QObject* parent = nullptr);

    Q_INVOKABLE AuthResult RegisterUser(const QString& username, const QString& email, const QString& password);
    Q_INVOKABLE AuthResult LoginUser(const QString& email, const QString& password);

signals:
    void registrationSuccess();
    void loginSuccess();
};