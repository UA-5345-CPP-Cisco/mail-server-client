#pragma once
#include <QSortFilterProxyModel>
#include "EmailListModel.h"
namespace ISXMail{

    class EmailFilterProxy : public QSortFilterProxyModel
    {
        Q_OBJECT
    public:
        enum FolderType { Inbox, Sent, Starred, Draft };
        Q_ENUM(FolderType)

        explicit EmailFilterProxy(FolderType type, QObject *parent = nullptr);

    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

    private:
        FolderType m_type;
    };

}