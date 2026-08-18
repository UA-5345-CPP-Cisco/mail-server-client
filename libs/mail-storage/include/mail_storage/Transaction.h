#pragma once

#include "Database.h"

namespace Storage {

/// RAII wrapper around a SQLite transaction.
///
/// Executes BEGIN IMMEDIATE on construction. If Commit() is not called before
/// the object goes out of scope (for example because an exception was thrown),
/// the destructor issues a ROLLBACK.
class Transaction
{
  public:
  explicit Transaction(Database& database);

  ~Transaction();

  Transaction(const Transaction&) = delete;

  Transaction& operator=(const Transaction&) = delete;

  Transaction(Transaction&&) = delete;

  Transaction& operator=(Transaction&&) = delete;

  /// Commits the transaction. After this call the destructor does nothing.
  void Commit();

  private:
  Database& m_database;

  bool m_committed{false};
};

} // namespace Storage
