#include "headers/database/CacheStore.h"

#include <chrono>
#include <iomanip>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <ctime>

#include "../../../../libs/mail-storage/include/mail_storage/Statement.h"
#include "../../../../libs/logger/include/logger/Logger.h"

namespace Storage {

namespace {
Logging::Logger kLogger(Logging::LogLevel::Debug, true);
constexpr const char* kCacheSchema = R"SQL(
	CREATE TABLE IF NOT EXISTS cache_entries (
		cache_namespace TEXT NOT NULL,
		cache_key TEXT NOT NULL,
		payload TEXT NOT NULL,
		version INTEGER NOT NULL DEFAULT 1,
		created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
		expires_at TEXT NOT NULL,
		PRIMARY KEY (cache_namespace, cache_key)
	);
	CREATE INDEX IF NOT EXISTS idx_cache_entries_expires_at
		ON cache_entries(expires_at);
)SQL";
}

CacheStore::CacheStore(Database& database) :
	m_database(database)
{
}

void CacheStore::EnsureSchema() const
{
	kLogger.Log(Logging::LogLevel::Info, "CacheStore::EnsureSchema: executing schema creation");
	m_database.Execute(kCacheSchema);
}

void CacheStore::Put(
	const std::string& cache_namespace,
	const std::string& cache_key,
	const std::string& payload,
	std::chrono::seconds ttl,
	std::int64_t version
)
{
	if (ttl.count() <= 0)
	{
		throw std::runtime_error("Cache TTL must be positive");
	}

	const auto expires_at = std::chrono::system_clock::now() + ttl;
	{
		std::ostringstream oss;
		oss << "CacheStore::Put: namespace=" << cache_namespace << " key=" << cache_key << " version=" << version << " ttl=" << ttl.count();
		kLogger.Log(Logging::LogLevel::Debug, oss.str());
	}

	Statement statement(
		m_database,
		R"SQL(
			INSERT INTO cache_entries (
				cache_namespace,
				cache_key,
				payload,
				version,
				expires_at
			)
			VALUES (?, ?, ?, ?, ?)
			ON CONFLICT(cache_namespace, cache_key)
			DO UPDATE SET
				payload = excluded.payload,
				version = excluded.version,
				created_at = CURRENT_TIMESTAMP,
				expires_at = excluded.expires_at;
		)SQL"
	);

	statement.BindText(1, cache_namespace);
	statement.BindText(2, cache_key);
	statement.BindText(3, payload);
	statement.BindInt64(4, version);
	statement.BindText(5, TimePointToString(expires_at));
	statement.Step();
	kLogger.Log(Logging::LogLevel::Debug, (std::string("CacheStore::Put: stored entry for namespace=") + cache_namespace + " key=" + cache_key));
}

std::optional<CacheStore::Entry> CacheStore::Get(
	const std::string& cache_namespace,
	const std::string& cache_key,
	bool allow_stale
) const
{
	Statement statement(
		m_database,
		R"SQL(
			SELECT
				cache_namespace,
				cache_key,
				payload,
				version,
				created_at,
				expires_at
			FROM cache_entries
			WHERE cache_namespace = ?
				AND cache_key = ?
			LIMIT 1;
		)SQL"
	);

	statement.BindText(1, cache_namespace);
	statement.BindText(2, cache_key);
	{
		std::ostringstream oss;
		oss << "CacheStore::Get: namespace=" << cache_namespace << " key=" << cache_key;
		kLogger.Log(Logging::LogLevel::Debug, oss.str());
	}

	if (!statement.Step())
	{
		kLogger.Log(Logging::LogLevel::Debug, (std::string("CacheStore::Get: miss for namespace=") + cache_namespace + " key=" + cache_key));
		return std::nullopt;
	}

	Entry entry = ReadEntry(statement);

	if (!allow_stale && entry.expires_at <= std::chrono::system_clock::now())
	{
		kLogger.Log(Logging::LogLevel::Debug, (std::string("CacheStore::Get: entry expired namespace=") + entry.cache_namespace + " key=" + entry.cache_key));
		return std::nullopt;
	}

	kLogger.Log(Logging::LogLevel::Debug, (std::string("CacheStore::Get: hit namespace=") + entry.cache_namespace + " key=" + entry.cache_key + " version=" + std::to_string(entry.version)));
	return entry;
}

bool CacheStore::Exists(
	const std::string& cache_namespace,
	const std::string& cache_key
) const
{
	return Get(cache_namespace, cache_key, true).has_value();
}

void CacheStore::Invalidate(
	const std::string& cache_namespace,
	const std::string& cache_key
)
{
	Statement statement(
		m_database,
		R"SQL(
			DELETE FROM cache_entries
			WHERE cache_namespace = ?
				AND cache_key = ?;
		)SQL"
	);

	statement.BindText(1, cache_namespace);
	statement.BindText(2, cache_key);
	kLogger.Log(Logging::LogLevel::Debug, (std::string("CacheStore::Invalidate: deleting namespace=") + cache_namespace + " key=" + cache_key));
	statement.Step();
}

void CacheStore::InvalidateNamespace(const std::string& cache_namespace)
{
	Statement statement(
		m_database,
		R"SQL(
			DELETE FROM cache_entries
			WHERE cache_namespace = ?;
		)SQL"
	);

	statement.BindText(1, cache_namespace);
		kLogger.Log(Logging::LogLevel::Debug, (std::string("CacheStore::InvalidateNamespace: deleting namespace=") + cache_namespace));
		statement.Step();
}

void CacheStore::InvalidateNamespacePrefix(
	const std::string& cache_namespace_prefix
)
{
	Statement statement(
		m_database,
		R"SQL(
			DELETE FROM cache_entries
			WHERE cache_namespace LIKE ? ESCAPE '\';
		)SQL"
	);

	statement.BindText(1, EscapeLike(cache_namespace_prefix) + "%");
		kLogger.Log(Logging::LogLevel::Debug, (std::string("CacheStore::InvalidateNamespacePrefix: prefix=") + cache_namespace_prefix));
		statement.Step();
}

CacheStore::Entry CacheStore::ReadEntry(const Statement& statement) const
{
	Entry entry;
	entry.cache_namespace = statement.ColumnText(0);
	entry.cache_key = statement.ColumnText(1);
	entry.payload = statement.ColumnText(2);
	entry.version = statement.ColumnInt64(3);
	entry.created_at = StringToTimePoint(statement.ColumnText(4));
	entry.expires_at = StringToTimePoint(statement.ColumnText(5));
	return entry;
}

std::string CacheStore::TimePointToString(
	const std::chrono::system_clock::time_point& time_point
    ) const
{
    const auto time_t_value = std::chrono::system_clock::to_time_t(time_point);
    std::tm tm{};
#ifdef _WIN32
    gmtime_s(&tm, &time_t_value); // Uses UTC/GMT instead of local time
#else
    gmtime_r(&time_t_value, &tm); // Uses UTC/GMT instead of local time
#endif
    std::ostringstream stream;
    stream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return stream.str();
}

std::chrono::system_clock::time_point CacheStore::StringToTimePoint(
	const std::string& text
    ) const
{
    std::tm tm{};
    std::istringstream stream(text);
    stream >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (stream.fail())
    {
        kLogger.Log(Logging::LogLevel::Error, (std::string("CacheStore::StringToTimePoint: Failed to parse cache timestamp: ") + text));
        throw std::runtime_error("Failed to parse cache timestamp: " + text);
    }

    // Force tm_isdst to -1 so the parser attempts to compute DST automatically
    tm.tm_isdst = -1;

#ifdef _WIN32
    // _mkgmtime is the Windows specific function to parse tm as UTC
    const time_t time_value = _mkgmtime(&tm);
#else
    // timegm is the POSIX/Linux standard equivalent to parse tm as UTC
    const time_t time_value = timegm(&tm);
#endif

    if (time_value == -1)
    {
        kLogger.Log(Logging::LogLevel::Error, "CacheStore::StringToTimePoint: Failed to convert broken-down time to UTC time_t");
        throw std::runtime_error("Failed to convert broken-down time to UTC time_t");
    }

    return std::chrono::system_clock::from_time_t(time_value);
}

std::string CacheStore::EscapeLike(const std::string& value) const
{
	std::string escaped;
	escaped.reserve(value.size() * 2);

	for (char ch : value)
	{
		if (ch == '%' || ch == '_' || ch == '\\')
		{
			escaped.push_back('\\');
		}

		escaped.push_back(ch);
	}

	return escaped;
}

}
