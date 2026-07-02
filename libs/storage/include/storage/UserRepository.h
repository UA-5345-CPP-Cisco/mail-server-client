#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "Database.h"
#include "UserRecord.h"

namespace Storage {

class Statement;

class UserRepository
{
  public:
  explicit UserRepository(Database& database);

  std::optional<UserRecord> FindById(std::int64_t user_id) const;

  std::optional<UserRecord> FindByEmail(const std::string& email) const;

  std::optional<UserRecord> FindByUsername(const std::string& username) const;

  std::int64_t CreateUser(const std::string& username,
                          const std::string& email,
                          const std::string& password_hash);

  std::optional<UserRecord> FindActiveUser() const;

  std::vector<UserRecord> FindAll() const;

  UserRecord ReadUser(const Statement& statement) const;

  std::string StatusToString(UserStatus status) const;

  UserStatus StatusFromString(const std::string& status) const;

  bool UpdateStatus(std::int64_t user_id, UserStatus status);

  bool HasUsers();

  private:
  Database& m_database;
};

} // namespace Storage
