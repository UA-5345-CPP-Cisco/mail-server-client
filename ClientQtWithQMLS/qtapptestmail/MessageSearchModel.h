#ifndef MESSAGESEARCHMODEL_H
#define MESSAGESEARCHMODEL_H

#include <QSortFilterProxyModel>
#include <QString>

class MessageSearchModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString getSearchedText READ getSearchedText WRITE setSearchedText NOTIFY searchedTextChanged)

public:
    explicit MessageSearchModel(QObject* parent = nullptr);
    QString getSearchedText();
    void setSearchedText(const QString& text);
    Q_INVOKABLE QString highlightAllFoundWords(const QString &text, const QString& searchedText) const;
signals:
    void searchedTextChanged();
private :
    QString _searchedText;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // MESSAGESEARCHMODEL_H
