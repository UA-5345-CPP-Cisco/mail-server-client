#include "headers/mail/EmailFilterProxy.h"

#include "headers/service/Service.h"

namespace ISXMail{

    namespace
    {

    }

    EmailFilterProxy::EmailFilterProxy(FolderType type, QObject *parent)
        : QSortFilterProxyModel(parent), m_type(type)
    {
         ISXService::Service::Logger().Log(Logging::LogLevel::Debug, "EmailFilterProxy: constructed");
        setDynamicSortFilter(true);
    }

    bool EmailFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
    {
        auto* src = sourceModel();
        if (!src) return false;

        const QModelIndex idx = src->index(sourceRow, 0, sourceParent);

        const bool is_inbox   = src->data(idx, InboxRole).toBool();
        const bool is_sent    = src->data(idx, SentRole).toBool();
        const bool is_draft   = src->data(idx, DraftRole).toBool();
        const bool is_starred = src->data(idx, StarredRole).toBool();

        switch (m_type) {
            case Inbox:   return is_inbox || (!is_sent && !is_draft);
            case Sent:    return is_sent && !is_inbox;
            case Starred: return  is_starred;
            case Draft:   return  is_draft;
        }
        return true;
    }
}
