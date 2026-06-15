#include "MessageSearchModel.h"
#include "emaillistmodel.h"

MessageSearchModel::MessageSearchModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

QString MessageSearchModel::getSearchedText()
{
    return _searchedText;
}

void MessageSearchModel::setSearchedText(const QString& text)
{
    if (_searchedText == text)
        return;

    beginFilterChange();
    _searchedText = text;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
    emit searchedTextChanged();
}

bool MessageSearchModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QString word = _searchedText.trimmed();

    if (word.isEmpty())
        return true;

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    QString theme = sourceModel()->data(index, EmailListModel::ThemeRole).toString();
    QString name = sourceModel()->data(index, EmailListModel::NameRole).toString();
    QString preview = sourceModel()->data(index, EmailListModel::PreviewRole).toString();

    return theme.contains(word, Qt::CaseInsensitive)
           || name.contains(word, Qt::CaseInsensitive)
           || preview.contains(word, Qt::CaseInsensitive);
}

QString MessageSearchModel::highlightAllFoundWords(const QString& text,const QString& searchedText) const
{
    QString word = searchedText.trimmed();

    if (word.isEmpty())
        return text.toHtmlEscaped();

    QString result;
    int start = 0;
    int found = text.indexOf(word, start, Qt::CaseInsensitive);

    while (found != -1)
    {
        result += text.mid(start, found - start).toHtmlEscaped();

        result += "<span style=\"background-color:#fff3a3;color:#101828\">";
        result += text.mid(found, word.length()).toHtmlEscaped();
        result += "</span>";

        start = found + word.length();
        found = text.indexOf(word, start, Qt::CaseInsensitive);
    }

    result += text.mid(start).toHtmlEscaped();

    return result;
}