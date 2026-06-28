#include "headers/search/MessageSearchModel.h"

#include <QVariant>

#include "headers/mail/EmailListModel.h"

namespace ISXMail
{

MessageSearchModel::MessageSearchModel(QObject* parent) :
	QSortFilterProxyModel(parent)
{
	setFilterCaseSensitivity(Qt::CaseInsensitive);
}

QString MessageSearchModel::get_searched_text() const
{
	return m_searched_text;
}

void MessageSearchModel::set_searched_text(const QString& text)
{
	if (m_searched_text == text)
	{
		return;
	}

	beginFilterChange();
	m_searched_text = text;
	endFilterChange(QSortFilterProxyModel::Direction::Rows);
	emit SearchedTextChanged();
}

bool MessageSearchModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	const QString word = m_searched_text.trimmed();

	if (word.isEmpty())
	{
		return true;
	}

	auto* source_model = sourceModel();
	if (!source_model)
	{
		return false;
	}

	const QModelIndex source_index = source_model->index(source_row, 0, source_parent);

	return TextMatches(source_model->data(source_index, ThemeRole).toString(), word)
		|| TextMatches(source_model->data(source_index, NameRole).toString(), word)
		|| TextMatches(source_model->data(source_index, PreviewRole).toString(), word)
		|| TextMatches(source_model->data(source_index, ContentRole).toString(), word)
		|| TextMatches(source_model->data(source_index, SendToRole).toString(), word)
		|| TextMatches(source_model->data(source_index, TimeRole).toString(), word);
}

QString MessageSearchModel::HighlightAllFoundWords(
	const QString& text,
	const QString& searched_text
) const
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

}
