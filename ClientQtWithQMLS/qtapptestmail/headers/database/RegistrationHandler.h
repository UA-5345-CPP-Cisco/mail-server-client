#pragma once
#include <QObject>
#include <QString>
#include "Database.h"

class RegistrationHandler : public QObject {
    Q_OBJECT
public:
    explicit RegistrationHandler(Storage::Database& db, QObject* parent = nullptr);

    Q_INVOKABLE bool registerUser(const QString& username, const QString& email, const QString& password);

signals:
    void registrationSuccess();

private:
    Storage::Database& m_db;
};