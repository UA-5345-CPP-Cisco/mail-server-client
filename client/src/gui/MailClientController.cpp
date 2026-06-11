#include "mail_client/gui/MailClientController.hpp"

#include <QMetaObject>
#include <QRegularExpression>
#include <QStringList>

#include <algorithm>
#include <exception>
#include <functional>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace mail_client::gui {

namespace {

StartTlsMode ToStartTlsMode(int value)
{
    if (value <= 0) {
        return StartTlsMode::Disabled;
    }
    if (value >= 2) {
        return StartTlsMode::Required;
    }
    return StartTlsMode::WhenAvailable;
}

std::string ToStdString(const QString& value)
{
    return value.trimmed().toStdString();
}

std::vector<Mailbox> ParseRecipients(const QString& value)
{
    std::vector<Mailbox> recipients;
    const QStringList parts = value.split(QRegularExpression("[,;]"), Qt::SkipEmptyParts);
    recipients.reserve(static_cast<std::size_t>(parts.size()));

    for (const QString& part : parts) {
        const QString address = part.trimmed();
        if (!address.isEmpty()) {
            recipients.push_back(Mailbox{address.toStdString(), {}});
        }
    }

    return recipients;
}

void RunInBackground(QObject* receiver,
                     std::function<QString()> work,
                     std::function<void(QString, bool)> finish)
{
    std::thread([receiver, work = std::move(work), finish = std::move(finish)]() mutable {
        bool succeeded = true;
        QString message;

        try {
            message = work();
        } catch (const std::exception& error) {
            succeeded = false;
            message = QString::fromStdString(error.what());
        } catch (...) {
            succeeded = false;
            message = "Unexpected error";
        }

        QMetaObject::invokeMethod(receiver,
                                  [finish = std::move(finish), message, succeeded]() mutable {
                                      finish(message, succeeded);
                                  },
                                  Qt::QueuedConnection);
    }).detach();
}

}

MailClientController::MailClientController(QObject* parent)
    : QObject(parent)
{
}

QString MailClientController::serverHost() const
{
    return serverHost_;
}

void MailClientController::setServerHost(const QString& value)
{
    const QString next = value.trimmed();
    if (serverHost_ == next) {
        return;
    }
    serverHost_ = next;
    emit settingsChanged();
}

int MailClientController::serverPort() const
{
    return serverPort_;
}

void MailClientController::setServerPort(int value)
{
    const int next = std::clamp(value, 1, 65535);
    if (serverPort_ == next) {
        return;
    }
    serverPort_ = next;
    emit settingsChanged();
}

QString MailClientController::clientName() const
{
    return clientName_;
}

void MailClientController::setClientName(const QString& value)
{
    const QString next = value.trimmed();
    if (clientName_ == next) {
        return;
    }
    clientName_ = next.isEmpty() ? QString{"mail-client.local"} : next;
    emit settingsChanged();
}

QString MailClientController::senderAddress() const
{
    return senderAddress_;
}

void MailClientController::setSenderAddress(const QString& value)
{
    const QString next = value.trimmed();
    if (senderAddress_ == next) {
        return;
    }
    senderAddress_ = next;
    emit settingsChanged();
}

QString MailClientController::senderName() const
{
    return senderName_;
}

void MailClientController::setSenderName(const QString& value)
{
    const QString next = value.trimmed();
    if (senderName_ == next) {
        return;
    }
    senderName_ = next;
    emit settingsChanged();
}

QString MailClientController::username() const
{
    return username_;
}

void MailClientController::setUsername(const QString& value)
{
    const QString next = value.trimmed();
    if (username_ == next) {
        return;
    }
    username_ = next;
    emit settingsChanged();
}

QString MailClientController::password() const
{
    return password_;
}

void MailClientController::setPassword(const QString& value)
{
    if (password_ == value) {
        return;
    }
    password_ = value;
    emit settingsChanged();
}

int MailClientController::tlsMode() const
{
    return tlsMode_;
}

void MailClientController::setTlsMode(int value)
{
    const int next = std::clamp(value, 0, 2);
    if (tlsMode_ == next) {
        return;
    }
    tlsMode_ = next;
    emit settingsChanged();
}

bool MailClientController::busy() const
{
    return busy_;
}

QString MailClientController::statusMessage() const
{
    return statusMessage_;
}

void MailClientController::checkConnection()
{
    if (busy_) {
        setStatusMessage("Another operation is already running");
        return;
    }

    const Snapshot current = snapshot();
    setBusy(true);
    setStatusMessage("Checking server...");

    RunInBackground(
        this,
        [current]() {
            SmtpClient client(current.settings);
            client.Connect();
            if (!current.username.isEmpty()) {
                client.Authenticate(SmtpCredentials{ToStdString(current.username),
                                                    current.password.toStdString()});
            }
            const SmtpReply reply = client.Noop();
            client.Disconnect();
            return QString{"Connected: %1 %2"}.arg(reply.code).arg(QString::fromStdString(reply.Message()));
        },
        [this](const QString& message, bool succeeded) {
            setBusy(false);
            setStatusMessage(message);
            if (succeeded) {
                emit operationSucceeded(message);
            } else {
                emit operationFailed(message);
            }
        });
}

void MailClientController::sendMessage(const QString& recipients,
                                       const QString& subject,
                                       const QString& body)
{
    if (busy_) {
        setStatusMessage("Another operation is already running");
        return;
    }

    const Snapshot current = snapshot();
    const std::vector<Mailbox> parsedRecipients = ParseRecipients(recipients);
    if (parsedRecipients.empty()) {
        setStatusMessage("Add at least one recipient");
        emit operationFailed(statusMessage_);
        return;
    }

    setBusy(true);
    setStatusMessage("Sending message...");

    RunInBackground(
        this,
        [current, parsedRecipients, subject, body]() {
            EmailMessage message;
            message.from = Mailbox{ToStdString(current.senderAddress), ToStdString(current.senderName)};
            message.to = parsedRecipients;
            message.subject = ToStdString(subject);
            message.body = body.toStdString();

            SmtpClient client(current.settings);
            client.Connect();
            if (!current.username.isEmpty()) {
                client.Authenticate(SmtpCredentials{ToStdString(current.username),
                                                    current.password.toStdString()});
            }
            const DeliveryReport report = client.SendEmail(message);
            client.Disconnect();

            if (!report.serverMessageId.empty()) {
                return QString{"Message sent as %1"}.arg(QString::fromStdString(report.serverMessageId));
            }
            return QString{"Message sent"};
        },
        [this](const QString& message, bool succeeded) {
            setBusy(false);
            setStatusMessage(message);
            if (succeeded) {
                emit operationSucceeded(message);
            } else {
                emit operationFailed(message);
            }
        });
}

MailClientController::Snapshot MailClientController::snapshot() const
{
    Snapshot current;
    current.settings.host = ToStdString(serverHost_);
    current.settings.port = static_cast<std::uint16_t>(serverPort_);
    current.settings.clientName = ToStdString(clientName_);
    current.settings.startTlsMode = ToStartTlsMode(tlsMode_);
    current.settings.tlsVerificationMode = TlsVerificationMode::None;
    current.senderAddress = senderAddress_;
    current.senderName = senderName_;
    current.username = username_;
    current.password = password_;
    return current;
}

void MailClientController::setBusy(bool value)
{
    if (busy_ == value) {
        return;
    }
    busy_ = value;
    emit busyChanged();
}

void MailClientController::setStatusMessage(const QString& value)
{
    if (statusMessage_ == value) {
        return;
    }
    statusMessage_ = value;
    emit statusMessageChanged();
}

}
