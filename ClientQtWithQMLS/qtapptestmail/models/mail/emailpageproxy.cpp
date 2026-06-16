#include "headers/mail/emailpageproxy.h"
namespace ISXMail{

EmailPageProxy::EmailPageProxy(QObject* parent)
    : QSortFilterProxyModel(parent) {}

void EmailPageProxy::nextPage()
{
    if (m_current_page < m_page_count - 1) {
        beginFilterChange();
        ++m_current_page;
        endFilterChange();
        emit currentPageChanged();
        emit pageAmountTextChanged();
    }
}

void EmailPageProxy::prevPage()
{
    if (m_current_page > 0) {
        beginFilterChange();
        --m_current_page;
        endFilterChange();
        emit currentPageChanged();
        emit pageAmountTextChanged();
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
    if (!sourceModel())
        return;

    beginFilterChange();

    bool amountTextDirty = false;

    int newEmailCount = sourceModel()->rowCount();
    if (newEmailCount != m_emails_count)
    {
        m_emails_count = newEmailCount;
        emit totalEmailsCountChanged();
        amountTextDirty = true;
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
        amountTextDirty = true;
    }

    if (amountTextDirty)
        emit pageAmountTextChanged();

    endFilterChange();
}
int EmailPageProxy::totalEmailsCount() const
{
    return m_emails_count;
}

QString EmailPageProxy::pageAmountText() const
{
    if (m_emails_count == 0)
        return QStringLiteral("0-0 of 0");

    int start_idx = m_current_page * m_per_page + 1;
    int end_idx   = qMin(m_emails_count, (m_current_page + 1) * m_per_page);
    return QStringLiteral("%1-%2 of %3").arg(start_idx).arg(end_idx).arg(m_emails_count);
}
}
