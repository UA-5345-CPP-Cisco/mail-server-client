#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <QString>

#include "headers/database/CacheStore.h"

namespace ISXMail {

struct CachedEmailList
{
	QString folder;
	QString search_text;
	QString payload;
	std::int64_t version{1};
};

class EmailListCache
{
public:
	explicit EmailListCache(Storage::CacheStore& store);

	void Save(
		const QString& folder,
		const QString& search_text,
		const QString& payload,
		std::chrono::seconds ttl = std::chrono::minutes(5),
		std::int64_t version = 1
	);
	std::optional<CachedEmailList> Load(
		const QString& folder,
		const QString& search_text,
		bool allow_stale = false
	) const;
	void InvalidateFolder(const QString& folder);
	void InvalidateAll();

private:
	Storage::CacheStore& m_store;

	QString Namespace() const;
	QString MakeKey(const QString& folder, const QString& search_text) const;
	QString MakeNamespace(const QString& folder) const;
};

}
