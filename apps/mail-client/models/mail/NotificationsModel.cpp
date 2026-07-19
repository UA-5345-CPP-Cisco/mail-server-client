#include "headers/mail/NotificationsModel.h"

#include <QCoreApplication>
#include <QGuiApplication>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QDebug>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QWindow>
#include "headers/service/Service.h"

namespace ISXMail
{

NotificationsModel::NotificationsModel(QObject* parent) :
    QObject(parent)
{
    ISXService::Service::Logger().Log(Logging::LogLevel::Info, "NotificationsModel constructed");
}

void NotificationsModel::showNotification(const QString& title, const QString& body, int timeoutMs)
{
    ISXService::Service::Logger().Log(Logging::LogLevel::Info, "showNotification: " + title.toStdString() + " - " + body.toStdString() + " timeout=" + std::to_string(timeoutMs));

    // QSystemTrayIcon requires QGuiApplication/QApplication.
    // In unit tests or CLI context, we may only have a QCoreApplication.
    if (!qobject_cast<QGuiApplication*>(QCoreApplication::instance()))
    {
        qDebug() << "NotificationsModel: GUI not running. Title:" << title << "Body:" << body;
        return;
    }

    static QSystemTrayIcon* trayIcon = nullptr;
    if (!trayIcon)
    {
        trayIcon = new QSystemTrayIcon(QCoreApplication::instance());
        // Try to load the app icon, otherwise use default
        QIcon icon(":/pngs/assets/Icon.png");
        if (icon.isNull())
        {
            icon = QIcon(":/pngs/assets/ic_inbox.svg");
        }
        trayIcon->setIcon(icon);

        // Context Menu with "Quit" option so the user can fully exit the background app
        QMenu* menu = new QMenu();
        QAction* quitAction = menu->addAction("Quit");
        QObject::connect(quitAction, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);
        trayIcon->setContextMenu(menu);

        // Restore/Show the main window when clicking/double-clicking the tray icon
        QObject::connect(trayIcon, &QSystemTrayIcon::activated, [](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
            {
                for (auto* window : QGuiApplication::topLevelWindows())
                {
                    window->show();
                    window->raise();
                    window->requestActivate();
                }
            }
        });

        // Restore/Show the main window when clicking the notification message itself
        QObject::connect(trayIcon, &QSystemTrayIcon::messageClicked, []() {
            for (auto* window : QGuiApplication::topLevelWindows())
            {
                window->show();
                window->raise();
                window->requestActivate();
            }
        });

        trayIcon->show();
    }

    trayIcon->showMessage(title, body, QSystemTrayIcon::Information, timeoutMs);
}

void NotificationsModel::quitAppAfterDelay(int delayMs)
{
    ISXService::Service::Logger().Log(Logging::LogLevel::Info, "quitAppAfterDelay: scheduling app exit in " + std::to_string(delayMs) + " ms");
    QTimer::singleShot(delayMs, QCoreApplication::instance(), &QCoreApplication::quit);
}

void NotificationsModel::onNewInboxMessage(const QString& sender, const QString& subject, const QString& preview)
{
    // Wait 10 seconds before triggering the notification
    ISXService::Service::Logger().Log(Logging::LogLevel::Info, "onNewInboxMessage: scheduling notification in 10 seconds");
    QTimer::singleShot(0, this, [this, sender, subject, preview]() {
        QString title = QString("New message from %1").arg(sender);
        QString body = subject;
        if (!preview.isEmpty())
        {
            body += "\n" + preview;
        }
        showNotification(title, body);
    });
}

} // namespace ISXMail
