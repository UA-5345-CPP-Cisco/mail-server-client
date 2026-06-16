#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QIcon>
#include <QQmlContext>
#include "headers\mail\emaillistmodel.h"
#include "headers\mail\emailfilterproxy.h"
#include "headers\mail\emailpageproxy.h"
#include "headers\users\currentuser.h"
#include "headers\search\messagesearchmodel.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_BACKEND", "software");
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("qrc:/pngs/assets/Icon.png"));
    QQmlApplicationEngine engine;

    auto* model = new ISXMail::EmailListModel(&app);

    auto* inboxFilter = new ISXMail::EmailFilterProxy(ISXMail::EmailFilterProxy::Inbox, &app);
    auto* sentFilter = new ISXMail::EmailFilterProxy(ISXMail::EmailFilterProxy::Sent, &app);
    auto* starredFilter = new ISXMail::EmailFilterProxy(ISXMail::EmailFilterProxy::Starred, &app);
    auto* draftFilter = new ISXMail::EmailFilterProxy(ISXMail::EmailFilterProxy::Draft, &app);

    inboxFilter->setSourceModel(model);
    sentFilter->setSourceModel(model);
    starredFilter->setSourceModel(model);
    draftFilter->setSourceModel(model);

    auto* inboxSearch = new ISXMail::MessageSearchModel(&app);
    auto* sentSearch = new ISXMail::MessageSearchModel(&app);
    auto* starredSearch = new ISXMail::MessageSearchModel(&app);
    auto* draftSearch = new ISXMail::MessageSearchModel(&app);

    inboxSearch->setSourceModel(inboxFilter);
    sentSearch->setSourceModel(sentFilter);
    starredSearch->setSourceModel(starredFilter);
    draftSearch->setSourceModel(draftFilter);

    auto* inbox = new ISXMail::EmailPageProxy(&app);
    auto* sent = new ISXMail::EmailPageProxy(&app);
    auto* starred = new ISXMail::EmailPageProxy(&app);
    auto* draft = new ISXMail::EmailPageProxy(&app);

    inbox->setSourceModel(inboxSearch);
    sent->setSourceModel(sentSearch);
    starred->setSourceModel(starredSearch);
    draft->setSourceModel(draftSearch);

    engine.rootContext()->setContextProperty("emailsModel", model);
    engine.rootContext()->setContextProperty("inboxModel", inbox);
    engine.rootContext()->setContextProperty("sentModel", sent);
    engine.rootContext()->setContextProperty("starredModel", starred);
    engine.rootContext()->setContextProperty("draftModel", draft);
    engine.rootContext()->setContextProperty("inboxSearchModel", inboxSearch);
    engine.rootContext()->setContextProperty("sentSearchModel", sentSearch);
    engine.rootContext()->setContextProperty("starredSearchModel", starredSearch);
    engine.rootContext()->setContextProperty("draftSearchModel", draftSearch);
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

