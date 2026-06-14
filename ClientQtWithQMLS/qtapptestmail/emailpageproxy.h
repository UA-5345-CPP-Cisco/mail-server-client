#ifndef EMAILPAGEPROXY_H
#define EMAILPAGEPROXY_H
#include <QSortFilterProxyModel>
class EmailPageProxy : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int currentPage READ currentPage NOTIFY currentPageChanged)
    Q_PROPERTY(int pageCount   READ pageCount   NOTIFY pageCountChanged)
    Q_PROPERTY(QString emailsCount READ emailsCount  NOTIFY emailsCountChanged)
public:
    explicit EmailPageProxy(QObject* parent = nullptr);

    Q_INVOKABLE void nextPage();
    Q_INVOKABLE void prevPage();

    int currentPage() const;
    int pageCount() const;
    QString emailsCount() const;

    void setSourceModel(QAbstractItemModel *sourceModel) override;
signals:
    void currentPageChanged();
    void pageCountChanged();
    void emailsCountChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex&) const override;

private:
    void recalcValues();

    int m_current_page = 0;
    int m_page_count   = 0;
    int m_emails_count = 0;
    static constexpr int m_per_page = 8;
};
#endif // EMAILPAGEPROXY_H
