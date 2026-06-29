#include "headers/search/MessageSearchModel.h"
#include "headers/mail/EmailListModel.h"

#include <QVariant>

namespace ISXMail
{

MessageSearchModel::MessageSearchModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

QString MessageSearchModel::SearchedText() const
{
    return m_searchedText;
}

void MessageSearchModel::SetSearchedText(const QString& text)
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

bool MessageSearchModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
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

    const QModelIndex index = src->index(source_row, 0, source_parent);

    return TextMatches(src->data(index,ThemeRole).toString(), word)
        || TextMatches(src->data(index, NameRole).toString(), word)
        || TextMatches(src->data(index, PreviewRole).toString(), word)
        || TextMatches(src->data(index, ContentRole).toString(), word)
        || TextMatches(src->data(index, SendToRole).toString(), word)
        || TextMatches(src->data(index, TimeRole).toString(), word);
}

QString MessageSearchModel::HighlightAllFoundWords(const QString& text, const QString& searched_text) const
{
    const QString word = searched_text.trimmed();

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

bool MessageSearchModel::TextMatches(const QString& candidate, const QString& word) const
{
    return candidate.contains(word, Qt::CaseInsensitive);
}

} // namespace ISXMail