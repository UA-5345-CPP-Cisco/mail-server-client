#include "mail_client/gui/MailClientController.hpp"

#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QGuiApplication>
#include <QIcon>
#include <QIODevice>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextStream>
#include <QTimer>
#include <QUrl>

namespace {

void InstallFileLogger()
{
    static QFile logFile(QCoreApplication::applicationDirPath() + "/mail-client-gui.log");
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext&, const QString& message) {
        if (!logFile.isOpen()) {
            return;
        }

        const char* level = "info";
        switch (type) {
        case QtDebugMsg:
            level = "debug";
            break;
        case QtInfoMsg:
            level = "info";
            break;
        case QtWarningMsg:
            level = "warning";
            break;
        case QtCriticalMsg:
            level = "critical";
            break;
        case QtFatalMsg:
            level = "fatal";
            break;
        }

        QTextStream stream(&logFile);
        stream << QDateTime::currentDateTime().toString(Qt::ISODate) << " [" << level << "] "
               << message << '\n';
        stream.flush();
    });
}

}

int main(int argc, char* argv[])
{
    qputenv("QT_QUICK_BACKEND", "software");
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");

    QGuiApplication app(argc, argv);
    InstallFileLogger();
    app.setWindowIcon(QIcon(":/qt/qml/MailClientGui/qml/assets/Icon.png"));

    mail_client::gui::MailClientController mailClient;

    QQmlApplicationEngine engine;
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.rootContext()->setContextProperty("mailClient", &mailClient);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.load(QUrl("qrc:/qt/qml/MailClientGui/qml/Main.qml"));

    QTimer::singleShot(0, &mailClient, [&mailClient]() {
        mailClient.checkConnection();
    });

    return QGuiApplication::exec();
}
