#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QIcon>
#include <QQmlContext>
#include "emaillistmodel.h"
#include "currentuser.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_BACKEND", "software");
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("qrc:/pngs/assets/Icon.png"));
    QQmlApplicationEngine engine;

    //emails model
    EmailListModel model{};
    engine.rootContext()->setContextProperty("emailsModel", &model);
    //current user in system
    engine.rootContext()->setContextProperty(
        "currentUser",
        &ISXCurrentUser::CurrentUser::get_instance()
        );

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("qtapptestmail", "Main");

    return QGuiApplication::exec();
}
