#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickStyle>
#include <QIcon>
#include <QQmlContext>

#include "headers/mail/EmailListModel.h"
#include "headers/mail/EmailFilterProxy.h"
#include "headers/mail/EmailPageProxy.h"
#include "headers/mail/MessageComposer.h"
#include "headers/database/DatabaseManager.h"
#include "headers/users/CurrentUser.h"
#include "headers/search/MessageSearchModel.h"
#include "headers/database/RegistrationHandler.h"
#include "headers/users/AccountListModel.h"
#include "../../libs/mail-storage/include/mail_storage/UserRepository.h"
#include "../../libs/logger/include/logger/Logger.h"


int main(int argc, char *argv[])
{
    qputenv("QT_QUICK_BACKEND", "software");
    QQuickStyle::setStyle(QStringLiteral("Fusion"));
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/pngs/assets/Icon.png"));

    ISXDatabaseManager::DatabaseManager::EnsureInitialized();
    auto dbPath = ISXDatabaseManager::DatabaseManager::DatabasePath();
    Storage::Database database(dbPath);
    QQmlApplicationEngine engine;

    Logging::Logger kLogger(Logging::LogLevel::Debug, true);

    RegistrationHandler regHandler(database);
    engine.rootContext()->setContextProperty("regHandler", &regHandler);

    Storage::UserRepository repo(database);
    bool hasUsers = repo.HasUsers();
    engine.rootContext()->setContextProperty("initialSetupRequired", !hasUsers);

    if (hasUsers) {
        auto activeUser = repo.FindActiveUser();

        if (activeUser.has_value())
        {
            QString name = QString::fromStdString(activeUser->username);
            QString email = QString::fromStdString(activeUser->email);
            ISXCurrentUser::CurrentUser::GetInstance().Authorize(name, email, "");
        }
    }

    auto* model = new ISXMail::EmailListModel(&app);
    auto* message_composer = new ISXMail::MessageComposer(&app);
    auto* account_model = new ISXMail::AccountListModel(&app);

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
    engine.rootContext()->setContextProperty("accountModel", account_model);
    engine.rootContext()->setContextProperty("inboxModel", inbox);
    engine.rootContext()->setContextProperty("sentModel", sent);
    engine.rootContext()->setContextProperty("starredModel", starred);
    engine.rootContext()->setContextProperty("draftModel", draft);
    engine.rootContext()->setContextProperty("inboxSearchModel", inboxSearch);
    engine.rootContext()->setContextProperty("sentSearchModel", sentSearch);
    engine.rootContext()->setContextProperty("starredSearchModel", starredSearch);
    engine.rootContext()->setContextProperty("draftSearchModel", draftSearch);
    engine.rootContext()->setContextProperty("MessageComposer", message_composer);

    engine.rootContext()->setContextProperty(
        "CurrentUser",
        &ISXCurrentUser::CurrentUser::GetInstance()
        );

    qmlRegisterUncreatableMetaObject(
        ISXMail::staticMetaObject,
        "ISXMail",
        1, 0,
        "EmailRole",
        "Not creatable"
    );

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { qCritical() << "QML object creation failed"; },
        Qt::QueuedConnection);
    engine.loadFromModule("qtapptestmail", "Main");

    return QGuiApplication::exec();
}