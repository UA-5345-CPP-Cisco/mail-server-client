#include "headers/mail/EmailFilterProxy.h"

namespace ISXMail{

EmailFilterProxy::EmailFilterProxy(FolderType type, QObject *parent) :
    QSortFilterProxyModel(parent),
    m_type(type)
{
    setDynamicSortFilter(true);
}

bool EmailFilterProxy::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    auto* src = sourceModel();
    if (!src)
    {
        return false;
    }

    const QModelIndex idx = src->index(source_row, 0, source_parent);

    const bool is_inbox   = src->data(idx, InboxRole).toBool();
    const bool is_sent    = src->data(idx, SentRole).toBool();
    const bool is_draft   = src->data(idx, DraftRole).toBool();
    const bool is_starred = src->data(idx, StarredRole).toBool();

    switch (m_type) {
    case Inbox:
        return is_inbox || (!is_sent && !is_draft);
    case Sent:
        return is_sent && !is_inbox;
    case Starred:
        return  is_starred;
    case Draft:
        return  is_draft;
    }

    return true;
}

}
