#pragma once

#include <QObject>
#include <QString>

#include "mail_storage/Database.h"

namespace ISXAuth {
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
  RegisterUser(const QString& username, const QString& email, const QString& password);
  Q_INVOKABLE AuthHandler::AuthResult LoginUser(const QString& email, const QString& password);

  Q_INVOKABLE QString get_last_logged_in_name() const;

  signals:
  void registrationSuccess();

  private:
  struct MockUser
  {
    std::string email;
    std::string username;
    std::string password_data;
  };
  Storage::Database& m_db;
  std::optional<MockUser> get_mock_user(const std::string& email);
  bool VerifyPassword(const QString& password, const std::string& stored_password_data);

  QString m_current_user_name;
};

} // namespace ISXAuth