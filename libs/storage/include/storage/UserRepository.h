#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "storage/Database.h"
#include "storage/UserRecord.h"

namespace Storage {

class Statement;

class UserRepository
{
  public:
  explicit UserRepository(Database& database);

  std::int64_t CreateUser(const std::string& username,
                          const std::string& email,
                          const std::string& password_hash);

  std::optional<UserRecord> FindById(std::int64_t user_id) const;

  std::optional<UserRecord> FindByEmail(const std::string& email) const;

  std::optional<UserRecord> FindByUsername(const std::string& username) const;

  bool UpdateStatus(std::int64_t user_id, UserStatus status);

  private:
  Database& m_database;

  UserRecord ReadUser(const Statement& statement) const;

  std::string StatusToString(UserStatus status) const;

  UserStatus StatusFromString(const std::string& status) const;
};

} // namespace Storage
