#pragma once

#include <QObject>
#include <QString>

#include "mail_storage/Database.h"

class AuthHandler : public QObject
{
  Q_OBJECT
  public:
  enum class AuthResult
  {
    Success,
    UserNotFound,
    WrongPassword,
    InternalError,
    DatabaseError,
    UserAlreadyExists
  };
  Q_ENUM(AuthResult)
  explicit AuthHandler(Storage::Database& db, QObject* parent = nullptr);

  Q_INVOKABLE bool
  registerUser(const QString& username, const QString& email, const QString& password);
  Q_INVOKABLE AuthHandler::AuthResult loginUser(const QString& email, const QString& password);

  Q_INVOKABLE QString getLastLoggedInName() const;

  signals:
  void registrationSuccess();

  private:
  struct MockUser
  {
    std::string email;
    std::string username;
    std::string passwordData;
  };
  Storage::Database& m_db;
  std::optional<MockUser> getMockUser(const std::string& email);
  bool verifyPassword(const QString& password, const std::string& storedHash);

  QString m_currentUserName;
};
