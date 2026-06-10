#include "emaillistmodel.h"

EmailListModel::EmailListModel(QObject *parent)
    : QAbstractListModel{parent}
    , m_current_page(0)
{}

int EmailListModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid()) return 0;
    int start = m_data_per_page * m_current_page;
    int remain = static_cast<int>(m_data.size()) - start;

    int result = qMax(0, qMin(m_data_per_page, remain));

    return result;
}

QVariant EmailListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return {};

    int real_index = m_data_per_page * m_current_page + index.row();

    if(real_index >= m_data.size()) return {};

    const auto& item = m_data[real_index];

    switch(role){
    case Role::ThemeRole: return item.theme;
    case Role::NameRole: return item.name;
    case Role::PreviewRole: return item.preview;
    case Role::TimeRole: return item.time;
    }

    return {};
}

QHash<int, QByteArray> EmailListModel::roleNames() const
{
    return {
        {StarredRole, "emailsStarred"},
        {ThemeRole, "emailsTheme"},
        {NameRole, "emailsName"},
        {PreviewRole, "emailsPreview"},
        {TimeRole, "emailsTime"}
    };
}

void EmailListModel::NextPage()
{
    if(m_current_page + 1 >= CountPage()) return;

    beginResetModel();
    ++m_current_page;
    endResetModel();

    emit currentPageChanged();
}

void EmailListModel::PrevPage()
{
    if(m_current_page == 0) return;

    beginResetModel();
    --m_current_page;
    endResetModel();

    emit currentPageChanged();
}

void EmailListModel::SetPage(int page)
{
    if(page < 0 || page >= CountPage()) return;

    beginResetModel();
    m_current_page = page;
    endResetModel();

    emit currentPageChanged();
}

void EmailListModel::AddData(bool is_starred, QString theme, QString name, QString preview, QString time)
{
    AddData({is_starred, theme, name, preview, time});
}

void EmailListModel::AddData(const EmailData &item)
{
    int row = m_data.size();
    m_data.push_back(item);

    if (row < m_data_per_page * (m_current_page + 1)) {
        int local_row = row - m_data_per_page * m_current_page;
        beginInsertRows(QModelIndex(), local_row, local_row);
        endInsertRows();
    }

    emit pageCountChanged();
}

int EmailListModel::CountPage() const
{
    return (m_data.size() + m_data_per_page - 1) / m_data_per_page;
}

int EmailListModel::CurrentPage() const
{
    return m_current_page;
}