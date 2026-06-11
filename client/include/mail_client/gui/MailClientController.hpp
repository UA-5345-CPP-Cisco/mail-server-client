#pragma once

#include "mail_client/SmtpClient.hpp"

#include <QObject>
#include <QString>

#include <cstdint>

namespace mail_client::gui {

class MailClientController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString serverHost READ serverHost WRITE setServerHost NOTIFY settingsChanged)
    Q_PROPERTY(int serverPort READ serverPort WRITE setServerPort NOTIFY settingsChanged)
    Q_PROPERTY(QString clientName READ clientName WRITE setClientName NOTIFY settingsChanged)
    Q_PROPERTY(QString senderAddress READ senderAddress WRITE setSenderAddress NOTIFY settingsChanged)
    Q_PROPERTY(QString senderName READ senderName WRITE setSenderName NOTIFY settingsChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY settingsChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY settingsChanged)
    Q_PROPERTY(int tlsMode READ tlsMode WRITE setTlsMode NOTIFY settingsChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit MailClientController(QObject* parent = nullptr);

    QString serverHost() const;
    void setServerHost(const QString& value);

    int serverPort() const;
    void setServerPort(int value);

    QString clientName() const;
    void setClientName(const QString& value);

    QString senderAddress() const;
    void setSenderAddress(const QString& value);

    QString senderName() const;
    void setSenderName(const QString& value);

    QString username() const;
    void setUsername(const QString& value);

    QString password() const;
    void setPassword(const QString& value);

    int tlsMode() const;
    void setTlsMode(int value);

    bool busy() const;
    QString statusMessage() const;

    Q_INVOKABLE void checkConnection();
    Q_INVOKABLE void sendMessage(const QString& recipients,
                                 const QString& subject,
                                 const QString& body);

signals:
    void settingsChanged();
    void busyChanged();
    void statusMessageChanged();
    void operationSucceeded(const QString& message);
    void operationFailed(const QString& message);

private:
    struct Snapshot {
        SmtpClientSettings settings;
        QString senderAddress;
        QString senderName;
        QString username;
        QString password;
    };

    Snapshot snapshot() const;
    void setBusy(bool value);
    void setStatusMessage(const QString& value);

    QString serverHost_{"127.0.0.1"};
    int serverPort_{2525};
    QString clientName_{"mail-client.local"};
    QString senderAddress_{"alice@example.com"};
    QString senderName_{"Alice"};
    QString username_;
    QString password_;
    int tlsMode_{1};
    bool busy_{false};
    QString statusMessage_{"Ready"};
};

}
