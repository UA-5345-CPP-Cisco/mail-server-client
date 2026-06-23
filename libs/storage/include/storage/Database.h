#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <sqlite3.h>

namespace Storage {

class Statement;

class Database
{
  public:
  explicit Database(const std::filesystem::path& path);
  ~Database() = default;

  Database(const Database&) = delete;
  Database& operator=(const Database&) = delete;

  Database(Database&&) noexcept = default;
  Database& operator=(Database&&) noexcept = default;

  void Execute(const std::string& sql);

  private:
  struct ConnectionDeleter
  {
    void operator()(sqlite3* connection) const noexcept;
  };

  using ConnectionPtr = std::unique_ptr<sqlite3, ConnectionDeleter>;

  ConnectionPtr m_connection;

  friend class Statement;

  void Configure();
};

} // namespace Storage
