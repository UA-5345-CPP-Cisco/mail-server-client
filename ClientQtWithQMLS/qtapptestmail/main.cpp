#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QIcon>
#include <QQmlContext>
#include "emaillistmodel.h"
#include "emailfilterproxy.h"
#include "emailpageproxy.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_BACKEND", "software");
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/qt/qml/qtapptestmail/assets/Icon.png"));
    QQmlApplicationEngine engine;

    auto* model = new EmailListModel(&app);

    auto* inboxFilter = new EmailFilterProxy(EmailFilterProxy::Inbox, &app);
    auto* sentFilter = new EmailFilterProxy(EmailFilterProxy::Sent, &app);
    auto* starredFilter = new EmailFilterProxy(EmailFilterProxy::Starred, &app);
    auto* draftFilter = new EmailFilterProxy(EmailFilterProxy::Draft, &app);

    inboxFilter->setSourceModel(model);
    sentFilter->setSourceModel(model);
    starredFilter->setSourceModel(model);
    draftFilter->setSourceModel(model);

    auto* inbox = new EmailPageProxy(&app);
    auto* sent = new EmailPageProxy(&app);
    auto* starred = new EmailPageProxy(&app);
    auto* draft = new EmailPageProxy(&app);

    inbox->setSourceModel(inboxFilter);
    sent->setSourceModel(sentFilter);
    starred->setSourceModel(starredFilter);
    draft->setSourceModel(draftFilter);

    engine.rootContext()->setContextProperty("emailsModel", model);
    engine.rootContext()->setContextProperty("inboxModel", inbox);
    engine.rootContext()->setContextProperty("sentModel", sent);
    engine.rootContext()->setContextProperty("starredModel", starred);
    engine.rootContext()->setContextProperty("draftModel", draft);


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("qtapptestmail", "Main");

    return QGuiApplication::exec();
}
