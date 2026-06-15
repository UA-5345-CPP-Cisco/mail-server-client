#ifndef EMAILFILTERPROXY_H
#define EMAILFILTERPROXY_H

#include <QSortFilterProxyModel>
#include "emaillistmodel.h"

class EmailFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    enum FolderType { Inbox, Sent, Starred, Draft };
    Q_ENUM(FolderType)

    explicit EmailFilterProxy(FolderType type, QObject *parent = nullptr);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    FolderType m_type;
};

#endif // EMAILFILTERPROXY_H
