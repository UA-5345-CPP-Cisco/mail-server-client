#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QIcon>
#include <QQmlContext>
#include "emaillistmodel.h"
#include "currentuser.h"
#include "MessageSearchModel.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_BACKEND", "software");
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("qrc:/pngs/assets/Icon.png"));
    QQmlApplicationEngine engine;

    //emails model
    EmailListModel model{};
    MessageSearchModel searchModel{};
    searchModel.setSourceModel(&model);
    engine.rootContext()->setContextProperty("emailsModel", &model);
    engine.rootContext()->setContextProperty("searchModel", &searchModel);
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
