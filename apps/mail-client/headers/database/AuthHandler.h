#pragma once

#include <QObject>
#include <QString>

class AuthHandler : public QObject
{
    Q_OBJECT
public:
    explicit AuthHandler(QObject* parent = nullptr);

    Q_INVOKABLE bool registerUser(const QString& username, const QString& email, const QString& password);
    Q_INVOKABLE bool loginUser(const QString& email, const QString& password);

signals:
    void registrationSuccess();
    void loginSuccess();
};
