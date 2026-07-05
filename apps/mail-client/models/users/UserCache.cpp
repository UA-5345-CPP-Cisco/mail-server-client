#include "headers/users/UserCache.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <stdexcept>

namespace ISXCurrentUser {

UserCache::UserCache(Storage::CacheStore& store) :
	m_store(store)
{
}

void UserCache::Save(
	const CachedUserProfile& profile,
	std::chrono::seconds ttl
)
{
	m_store.Put(
		Namespace().toStdString(),
		Key().toStdString(),
		Serialize(profile).toStdString(),
		ttl,
		profile.version
	);
}

std::optional<CachedUserProfile> UserCache::Load(bool allow_stale) const
{
	const auto entry = m_store.Get(
		Namespace().toStdString(),
		Key().toStdString(),
		allow_stale
	);

	if (!entry.has_value())
	{
		return std::nullopt;
	}

	return Deserialize(QString::fromStdString(entry->payload));
}

void UserCache::Invalidate()
{
	m_store.Invalidate(Namespace().toStdString(), Key().toStdString());
}

QString UserCache::Key() const
{
	return QStringLiteral("active-user");
}

QString UserCache::Namespace() const
{
	return QStringLiteral("user-profile");
}

QString UserCache::Serialize(const CachedUserProfile& profile) const
{
	QJsonObject object;
	object["username"] = profile.username;
	object["email"] = profile.email;
	object["avatar_path"] = profile.avatar_path;
	object["is_authorized"] = profile.is_authorized;
	object["version"] = static_cast<double>(profile.version);
	return QString::fromUtf8(QJsonDocument(object).toJson(QJsonDocument::Compact));
}

CachedUserProfile UserCache::Deserialize(const QString& payload) const
{
	const QJsonDocument document = QJsonDocument::fromJson(payload.toUtf8());

	if (!document.isObject())
	{
		throw std::runtime_error("Invalid cached user payload");
	}

	const QJsonObject object = document.object();
	CachedUserProfile profile;
	profile.username = object.value("username").toString();
	profile.email = object.value("email").toString();
	profile.avatar_path = object.value("avatar_path").toString();
	profile.is_authorized = object.value("is_authorized").toBool();
	profile.version = static_cast<std::int64_t>(object.value("version").toDouble(1));
	return profile;
}

}
