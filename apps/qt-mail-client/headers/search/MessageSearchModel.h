#pragma once

#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>

namespace ISXMail
{
class MessageSearchModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString searchedText READ searchedText WRITE setSearchedText NOTIFY searchedTextChanged)

public:
    explicit MessageSearchModel(QObject* parent = nullptr);
    QString searchedText() const;
    void setSearchedText(const QString& text);
    Q_INVOKABLE QString highlightAllFoundWords(const QString &text, const QString& searchedText) const;
signals:
    void searchedTextChanged();

private:
    QString m_searchedText;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool textMatches(const QString& candidate, const QString& word) const;
};
}
