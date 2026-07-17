#include "headers/database/RegistrationHandler.h"
#include "../../../../libs/mail-storage/include/mail_storage/UserRepository.h"

RegistrationHandler::RegistrationHandler(Storage::Database& db, QObject* parent) : QObject(parent), m_db(db) {}

bool RegistrationHandler::registerUser(const QString& username, const QString& email, const QString& password) {

    if (username.isEmpty() || email.isEmpty() || password.isEmpty())
        return false;

    Storage::UserRepository repo(m_db);

    // Transmit data directly without hashing
    std::int64_t id{-1};
    try
    {
      id = repo.CreateUser(username.toStdString(), email.toStdString(), password.toStdString());
    }
    catch (...)
    {
      // TODO: Logger with ERROR level about this possible issue
    }

    return id != -1;
}