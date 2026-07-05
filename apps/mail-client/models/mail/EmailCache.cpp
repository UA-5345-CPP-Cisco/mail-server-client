#include "headers/mail/EmailCache.h"
#include "../../../../libs/logger/include/logger/Logger.h"

namespace ISXMail {

EmailListCache::EmailListCache(Storage::CacheStore& store) :
	m_store(store)
{
	static Logging::Logger kLogger(Logging::LogLevel::Debug, false);
	kLogger.Log(Logging::LogLevel::Debug, "EmailListCache: constructed");
}

void EmailListCache::Save(
	const QString& folder,
	const QString& search_text,
	const QString& payload,
	std::chrono::seconds ttl,
	std::int64_t version
)
{
	{
	std::ostringstream oss;
	oss << "EmailListCache::Save: folder=" << folder.toStdString() << " search=" << search_text.toStdString() << " version=" << version << " ttl=" << ttl.count();
	static Logging::Logger kLogger(Logging::LogLevel::Debug, true);
	kLogger.Log(Logging::LogLevel::Debug, oss.str());

	m_store.Put(
		MakeNamespace(folder).toStdString(),
		MakeKey(folder, search_text).toStdString(),
		payload.toStdString(),
		ttl,
		version
	);
		}}

std::optional<CachedEmailList> EmailListCache::Load(
	const QString& folder,
	const QString& search_text,
	bool allow_stale
) const
{
	const auto entry = m_store.Get(
		MakeNamespace(folder).toStdString(),
		MakeKey(folder, search_text).toStdString(),
		allow_stale
	);

		static Logging::Logger kLogger(Logging::LogLevel::Debug, true);
		if (!entry.has_value())
		{
			kLogger.Log(Logging::LogLevel::Debug, (std::string("EmailListCache::Load: miss folder=") + folder.toStdString() + " search=" + search_text.toStdString()));
			return std::nullopt;
		}

		kLogger.Log(Logging::LogLevel::Debug, (std::string("EmailListCache::Load: hit folder=") + folder.toStdString() + " search=" + search_text.toStdString() + " version=" + std::to_string(entry->version)));
		return CachedEmailList{
			folder,
			search_text,
			QString::fromStdString(entry->payload),
			entry->version
		};
}

void EmailListCache::InvalidateFolder(const QString& folder)
{
	{
	static Logging::Logger kLogger(Logging::LogLevel::Debug, true);
	kLogger.Log(Logging::LogLevel::Debug, (std::string("EmailListCache::InvalidateFolder: folder=") + folder.toStdString()));
	m_store.InvalidateNamespace(MakeNamespace(folder).toStdString());
}
}

void EmailListCache::InvalidateAll()
{
	{
	static Logging::Logger kLogger(Logging::LogLevel::Debug, true);
	kLogger.Log(Logging::LogLevel::Debug, "EmailListCache::InvalidateAll");
	m_store.InvalidateNamespacePrefix(Namespace().toStdString());
}
}

QString EmailListCache::Namespace() const
{
	return QStringLiteral("email-list");
}

QString EmailListCache::MakeKey(const QString& folder, const QString& search_text) const
{
	return folder.trimmed().toLower() + QStringLiteral("|") + search_text.trimmed().toLower();
}

QString EmailListCache::MakeNamespace(const QString& folder) const
{
	return Namespace() + QStringLiteral(":") + folder.trimmed().toLower();
}

}
