#pragma once

#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>

namespace ISXMail
{
class MessageSearchModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString SearchedText READ SearchedText WRITE SetSearchedText NOTIFY searchedTextChanged)

public:
    explicit MessageSearchModel(QObject* parent = nullptr);
    QString SearchedText() const;
    void SetSearchedText(const QString& text);
    Q_INVOKABLE QString HighlightAllFoundWords(const QString &text, const QString& searched_text) const;
signals:
    void searchedTextChanged();

private:
    QString m_searchedText;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool TextMatches(const QString& candidate, const QString& word) const;
};
}
