#include "headers/mail/EmailCache.h"

namespace ISXMail {

EmailListCache::EmailListCache(Storage::CacheStore& store) : m_store(store)
{
}

void EmailListCache::Save(const QString& folder,
                          const QString& search_text,
                          const QString& payload,
                          std::chrono::seconds ttl,
                          std::int64_t version)
{
  m_store.Put(MakeNamespace(folder).toStdString(),
              MakeKey(folder, search_text).toStdString(),
              payload.toStdString(),
              ttl,
              version);
}

std::optional<CachedEmailList>
EmailListCache::Load(const QString& folder, const QString& search_text, bool allow_stale) const
{
  const auto entry = m_store.Get(
    MakeNamespace(folder).toStdString(), MakeKey(folder, search_text).toStdString(), allow_stale);

  if (!entry.has_value())
  {
    return std::nullopt;
  }

  return CachedEmailList{
    folder, search_text, QString::fromStdString(entry->payload), entry->version};
}

void EmailListCache::InvalidateFolder(const QString& folder)
{
  m_store.InvalidateNamespace(MakeNamespace(folder).toStdString());
}

void EmailListCache::InvalidateAll()
{
  m_store.InvalidateNamespacePrefix(Namespace().toStdString());
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

} // namespace ISXMail
