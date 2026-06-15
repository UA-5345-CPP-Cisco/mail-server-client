#ifndef EMAILLISTMODEL_H
#define EMAILLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <vector>
#include <QtMath>
#include <QString>

struct EmailData
{
    bool is_starred;
    bool is_sent;
    bool is_draft;
    QString theme;
    QString name;
    QString sendTo;
    QString preview;
    QString content;
    QString time;
};

enum EmailRole
{
    StarredRole = Qt::UserRole+1,
    SentRole,
    DraftRole,
    ThemeRole,
    NameRole,
    SendToRole,
    PreviewRole,
    ContentRole,
    TimeRole
};

class EmailListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUM(EmailRole)

public:
    explicit EmailListModel(QObject *parent = nullptr);

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void AddData(bool is_starred, bool is_sent, bool is_draft,
                             const QString& theme, const QString& name,
                             const QString& sendTo, const QString& content, const QString& time);
    void AddData(const EmailData& item);

signals:
    void dataAdded();
private:
    QString makePreview(const QString& text, int maxLen = 50);
    std::vector<EmailData> m_data;
};


#endif // EMAILLISTMODEL_H
