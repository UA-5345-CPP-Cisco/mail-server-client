#pragma once

#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>

namespace ISXMail {

class MessageSearchModel : public QSortFilterProxyModel
{
	Q_OBJECT
	Q_PROPERTY(QString searchedText READ get_searched_text WRITE set_searched_text NOTIFY SearchedTextChanged)

public:
	explicit MessageSearchModel(QObject* parent = nullptr);

	QString get_searched_text() const;
	void set_searched_text(const QString& text);
	Q_INVOKABLE QString HighlightAllFoundWords(
		const QString& text,
		const QString& searched_text
	) const;

signals:
	void SearchedTextChanged();

private:
	QString m_searched_text;

	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
	bool TextMatches(const QString& candidate, const QString& word) const;
};

}
