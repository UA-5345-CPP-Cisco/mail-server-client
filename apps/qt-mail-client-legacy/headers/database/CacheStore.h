#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>

#include "../../../../libs/storage/include/storage/Database.h"

namespace Storage {

class Statement;

class CacheStore
{
  public:
  struct Entry
  {
    std::string cache_key;
    std::string cache_namespace;
    std::string payload;
    std::int64_t version{0};
    std::chrono::system_clock::time_point created_at;
    std::chrono::system_clock::time_point expires_at;
  };

  explicit CacheStore(Database& database);

  void EnsureSchema() const;

  void Put(const std::string& cache_namespace,
           const std::string& cache_key,
           const std::string& payload,
           std::chrono::seconds ttl,
           std::int64_t version = 1);

  std::optional<Entry> Get(const std::string& cache_namespace,
                           const std::string& cache_key,
                           bool allow_stale = false) const;

  bool Exists(const std::string& cache_namespace, const std::string& cache_key) const;

  void Invalidate(const std::string& cache_namespace, const std::string& cache_key);

  void InvalidateNamespace(const std::string& cache_namespace);

  void InvalidateNamespacePrefix(const std::string& cache_namespace_prefix);

  private:
  Database& m_database;

  Entry ReadEntry(const Statement& statement) const;

  std::string TimePointToString(const std::chrono::system_clock::time_point& time_point) const;

  std::chrono::system_clock::time_point StringToTimePoint(const std::string& text) const;

  std::string EscapeLike(const std::string& value) const;
};

} // namespace Storage
