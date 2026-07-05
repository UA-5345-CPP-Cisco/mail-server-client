#pragma once

#include <chrono>
#include <cstdint>
#include <optional>

#include <QString>

#include "headers/database/CacheStore.h"

namespace ISXCurrentUser {

struct CachedUserProfile
{
	QString username;
	QString email;
	QString avatar_path;
	bool is_authorized{false};
	std::int64_t version{1};
};

class UserCache
{
public:
	explicit UserCache(Storage::CacheStore& store);

	void Save(
		const CachedUserProfile& profile,
		std::chrono::seconds ttl = std::chrono::minutes(30)
	);
	std::optional<CachedUserProfile> Load(bool allow_stale = false) const;
	void Invalidate();

private:
	Storage::CacheStore& m_store;

	QString Key() const;
	QString Namespace() const;
	QString Serialize(const CachedUserProfile& profile) const;
	CachedUserProfile Deserialize(const QString& payload) const;
};

}
