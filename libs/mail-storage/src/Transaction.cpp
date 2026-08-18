#include "../include/mail_storage/Transaction.h"

namespace Storage {

Transaction::Transaction(Database& database) : m_database(database)
{
  m_database.Execute("BEGIN IMMEDIATE;");
}

Transaction::~Transaction()
{
  if (m_committed)
  {
    return;
  }

  // A destructor is implicitly noexcept, so a failing ROLLBACK must not escape:
  // letting it propagate would call std::terminate, most likely while another
  // exception is already in flight.
  try
  {
    m_database.Execute("ROLLBACK;");
  }
  catch (...)
  {
  }
}

void Transaction::Commit()
{
  m_database.Execute("COMMIT;");
  m_committed = true;
}

} // namespace Storage
