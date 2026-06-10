#ifndef EMAILLISTMODEL_H
#define EMAILLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <vector>
#include <QtMath>
#include <QString>

struct EmailData{
    bool is_starred;
    QString theme;
    QString name;
    QString preview;
    QString time;
};

class EmailListModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Role{
        StarredRole = Qt::UserRole+1,
        ThemeRole,
        NameRole,
        PreviewRole,
        TimeRole
    };

    explicit EmailListModel(QObject *parent = nullptr);


    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void NextPage();
    Q_INVOKABLE void PrevPage();
    Q_INVOKABLE void SetPage(int page);
    Q_INVOKABLE void AddData(bool is_starred, QString theme, QString name, QString preview, QString time);

    void AddData(const EmailData& item);
    int CountPage() const;
    int CurrentPage() const;
signals:
    void currentPageChanged();
    void pageCountChanged();
private:
    std::vector<EmailData> m_data;
    int m_current_page;
    static constexpr int m_data_per_page = 8;
};

#endif // EMAILLISTMODEL_H
