#pragma once

#include <QObject>
#include <QString>

namespace ISXMail
{

class NotificationsModel : public QObject
{
    Q_OBJECT
public:
    explicit NotificationsModel(QObject* parent = nullptr);

    Q_INVOKABLE void showNotification(const QString& title, const QString& body, int timeoutMs = 10000);
    Q_INVOKABLE void quitAppAfterDelay(int delayMs = 10000);

public slots:
    void onNewInboxMessage(const QString& sender, const QString& subject, const QString& preview);
};

} // namespace ISXMail
