#include "emailpageproxy.h"

EmailPageProxy::EmailPageProxy(QObject* parent)
    : QSortFilterProxyModel(parent) {}

void EmailPageProxy::nextPage()
{
    if (m_current_page < m_page_count - 1) {
        beginFilterChange();
        ++m_current_page;
        endFilterChange();
        emit currentPageChanged();
    }
}

void EmailPageProxy::prevPage()
{
    if(m_current_page > 0)
    {
        beginFilterChange();
        --m_current_page;
        endFilterChange();
        emit currentPageChanged();
    }
}

int EmailPageProxy::currentPage() const
{
    return m_current_page;
}

int EmailPageProxy::pageCount() const
{
    if (!sourceModel())
        return 0;
    int total = sourceModel()->rowCount();
    return (total + m_per_page - 1) / m_per_page;
}

QString EmailPageProxy::emailsCount() const
{
    return QString::number(m_emails_count);
}

void EmailPageProxy::setSourceModel(QAbstractItemModel *sourceModel)
{
    QSortFilterProxyModel::setSourceModel(sourceModel);
    if (sourceModel)
    {
        connect(sourceModel,
                &QAbstractItemModel::rowsInserted,
                this,
                &EmailPageProxy::recalcValues);
        connect(sourceModel,
                &QAbstractItemModel::rowsRemoved,
                this,
                &EmailPageProxy::recalcValues);
        connect(sourceModel,
                &QAbstractItemModel::modelReset,
                this,
                &EmailPageProxy::recalcValues);
        connect(sourceModel,
                &QAbstractItemModel::layoutChanged,
                this,
                &EmailPageProxy::recalcValues);
    }
    recalcValues();
}

bool EmailPageProxy::filterAcceptsRow(int sourceRow, const QModelIndex&) const
{
    int start = m_current_page * m_per_page;
    int end   = start + m_per_page - 1;
    return sourceRow >= start && sourceRow <= end;
}

void EmailPageProxy::recalcValues()
{
    beginFilterChange();
    int newEmailCount = sourceModel()->rowCount();;
    if(newEmailCount != m_emails_count)
    {
        m_emails_count = newEmailCount;
        emit emailsCountChanged();
    }

    int newCount = pageCount();
    if (newCount != m_page_count)
    {
        m_page_count = newCount;
        emit pageCountChanged();
    }

    int clamped = m_page_count == 0 ? 0 : qMin(m_current_page, m_page_count - 1);
    if (clamped != m_current_page)
    {
        m_current_page = clamped;
        emit currentPageChanged();
    }
    endFilterChange();
}
