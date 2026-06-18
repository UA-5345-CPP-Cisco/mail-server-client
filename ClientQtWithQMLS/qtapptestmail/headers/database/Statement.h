#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <sqlite3.h>

#include "Database.h"

namespace Storage
{

class Statement
{
public:
	Statement(Database& database, const std::string& sql);
	~Statement() = default;

	Statement(const Statement&) = delete;
	Statement& operator=(const Statement&) = delete;

	Statement(Statement&&) noexcept = default;
	Statement& operator=(Statement&&) noexcept = default;

	void BindInt(int index, int value);
	void BindInt64(int index, std::int64_t value);
	void BindText(int index, const std::string& value);
	void BindNull(int index);

	bool Step();

	std::int64_t ColumnInt64(int index) const;
	std::string ColumnText(int index) const;
	bool ColumnIsNull(int index) const;

	std::int64_t LastInsertRowId() const;
	int ChangedRowCount() const;

private:
	struct StatementDeleter
	{
		void operator()(sqlite3_stmt* statement) const noexcept;
	};

	using StatementPtr = std::unique_ptr<sqlite3_stmt, StatementDeleter>;

	StatementPtr m_statement;

	void ThrowError(const std::string& message) const;
};

}
