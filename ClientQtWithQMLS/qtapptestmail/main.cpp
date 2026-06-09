#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QIcon>

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_BACKEND", "software");
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":assets/Icon.png"));

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("qtapptestmail", "Main");

    return QGuiApplication::exec();
}
