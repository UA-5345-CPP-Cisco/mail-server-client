#include "messagesearchmodel.h"
#include "emaillistmodel.h"
#include <QVariant>

namespace ISXMail
{

MessageSearchModel::MessageSearchModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

QString MessageSearchModel::searchedText() const
{
    return m_searchedText;
}

void MessageSearchModel::setSearchedText(const QString& text)
{
    if (m_searchedText == text)
    {
        return;
    }

    beginFilterChange();
    m_searchedText = text;
    endFilterChange(QSortFilterProxyModel::Direction::Rows);
    emit searchedTextChanged();
}

bool MessageSearchModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    const QString word = m_searchedText.trimmed();

    if (word.isEmpty())
    {
        return true;
    }

    auto* src = sourceModel();
    if (!src)
    {
        return false;
    }

    const QModelIndex index = src->index(sourceRow, 0, sourceParent);

    return textMatches(src->data(index,ThemeRole).toString(), word)
        || textMatches(src->data(index, NameRole).toString(), word)
        || textMatches(src->data(index, PreviewRole).toString(), word)
        || textMatches(src->data(index, ContentRole).toString(), word)
        || textMatches(src->data(index, SendToRole).toString(), word)
        || textMatches(src->data(index, TimeRole).toString(), word);
}

QString MessageSearchModel::highlightAllFoundWords(const QString& text, const QString& searchedText) const
{
    const QString word = searchedText.trimmed();

    if (word.isEmpty())
    {
        return text.toHtmlEscaped();
    }

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

bool MessageSearchModel::textMatches(const QString& candidate, const QString& word) const
{
    return candidate.contains(word, Qt::CaseInsensitive);
}

} // namespace ISXMail