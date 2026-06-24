#include "headers/mail/EmailPageProxy.h"
namespace ISXMail{

EmailPageProxy::EmailPageProxy(QObject* parent) :
    QSortFilterProxyModel(parent)
{
}

void EmailPageProxy::NextPage()
{
    if (m_current_page < m_page_count - 1)
    {
        beginFilterChange();
        ++m_current_page;
        endFilterChange();
        emit currentPageChanged();
        emit pageAmountTextChanged();
    }
}

void EmailPageProxy::PrevPage()
{
    if (m_current_page > 0)
    {
        beginFilterChange();
        --m_current_page;
        endFilterChange();
        emit currentPageChanged();
        emit pageAmountTextChanged();
    }
}
int EmailPageProxy::CurrentPage() const
{
    return m_current_page;
}

int EmailPageProxy::PageCount() const
{
    if (!sourceModel())
    {
        return 0;
    }

    int total = sourceModel()->rowCount();
    return (total + s_per_page - 1) / s_per_page;
}

void EmailPageProxy::setSourceModel(QAbstractItemModel *source_model)
{
    QSortFilterProxyModel::setSourceModel(source_model);

    if (source_model)
    {
        connect(source_model,
                &QAbstractItemModel::rowsInserted,
                this,
                &EmailPageProxy::RecalcValues);
        connect(source_model,
                &QAbstractItemModel::rowsRemoved,
                this,
                &EmailPageProxy::RecalcValues);
        connect(source_model,
                &QAbstractItemModel::modelReset,
                this,
                &EmailPageProxy::RecalcValues);
        connect(source_model,
                &QAbstractItemModel::layoutChanged,
                this,
                &EmailPageProxy::RecalcValues);
    }

    RecalcValues();
}

bool EmailPageProxy::filterAcceptsRow(int source_row, const QModelIndex&) const
{
    int start = m_current_page * s_per_page;
    int end   = start + s_per_page - 1;
    return source_row >= start && source_row <= end;
}

void EmailPageProxy::RecalcValues()
{
    if (!sourceModel())
    {
        return;
    }

    beginFilterChange();

    bool amount_text_dirty = false;

    int new_email_count = sourceModel()->rowCount();
    if (new_email_count != m_emails_count)
    {
        m_emails_count = new_email_count;
        emit totalEmailsCountChanged();
        amount_text_dirty = true;
    }

    int new_count = PageCount();
    if (new_count != m_page_count)
    {
        m_page_count = new_count;
        emit pageCountChanged();
    }

    int clamped = m_page_count == 0 ? 0 : qMin(m_current_page, m_page_count - 1);
    if (clamped != m_current_page)
    {
        m_current_page = clamped;
        emit currentPageChanged();
        amount_text_dirty = true;
    }

    if (amount_text_dirty)
    {
        emit pageAmountTextChanged();
    }

    endFilterChange();
}
int EmailPageProxy::TotalEmailsCount() const
{
    return m_emails_count;
}

QString EmailPageProxy::PageAmountText() const
{
    if (m_emails_count == 0)
    {
        return QStringLiteral("0-0 of 0");
    }

    int start_idx = m_current_page * s_per_page + 1;
    int end_idx   = qMin(m_emails_count, (m_current_page + 1) * s_per_page);
    return QStringLiteral("%1-%2 of %3").arg(start_idx).arg(end_idx).arg(m_emails_count);
}
bool EmailPageProxy::SetEmailData(int proxy_row, const QVariant& value, int role)
{
    QModelIndex proxyIndex = index(proxy_row, 0);
    QModelIndex sourceIndex = mapToSource(proxyIndex);

    auto* filterProxy = qobject_cast<QAbstractProxyModel*>(sourceModel());
    if (filterProxy) {
        QModelIndex modelIndex = filterProxy->mapToSource(sourceIndex);
        return filterProxy->sourceModel()->setData(modelIndex, value, role);
    }

    return false;
}

bool EmailPageProxy::SetStarred(int proxy_row, bool starred)
{
    return SetEmailData(proxy_row, starred, StarredRole);
}

void EmailPageProxy::RemoveEmailData(int proxy_row)
{
    QModelIndex idx = index(proxy_row, 0);

    while (auto* proxy = qobject_cast<QAbstractProxyModel*>(const_cast<QAbstractItemModel*>(idx.model())))
    {
        idx = proxy->mapToSource(idx);
    }

    auto* model = qobject_cast<EmailListModel*>(const_cast<QAbstractItemModel*>(idx.model()));

    if (!model || !idx.isValid())
    {
        return;
    }

    model->RemoveData(idx.row());
}

}

