#include "smtp/Server.hpp"
#include "logger/Logger.h"
#include "smtp/AuthService.hpp"
#include "smtp/Configuration.hpp"
#include "smtp/QueueDispatcher.hpp"
#include "smtp/SocketsManager.hpp"
#include "storage/Database.h"
#include "storage/MailMessageRepository.h"
#include "storage/MessageRecipientRepository.h"
#include "storage/MigrationRunner.h"
#include "storage/UserRepository.h"
#include "thread_pool/ThreadPool.h"

#include <cstdlib>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace
{

    std::string ReadEnv(const char *name, std::string fallback = {})
    {
        if (const char *value = std::getenv(name))
        {
            return value;
        }
        return fallback;
    }

}

int main(int argumentCount, char *arguments[])
{

    auto &logger = Logger::GetInstance();
    logger.Initialize("app.log", LogLevels::Trace, true);
    logger.LogProd("main", "Initializing SMTP Server...");

    try
    {
        const std::string configurationPath =
            argumentCount > 1 ? arguments[1] : "config/smtp_server.json";
        const smtp::Configuration configuration(configurationPath);

        smtp::BoostSocketsManager socketsManager;
        smtp::SmtpSessionHandler sessionHandler;
        smtp::AuthService authService;

        Storage::Database database(configuration.Database().storagePath);
        Storage::MigrationRunner migrationRunner(
            database,
            configuration.Database().migrationsPath);
        migrationRunner.Run();

        Storage::MailMessageRepository mailMessages(database);
        Storage::MessageRecipientRepository messageRecipients(database);
        Storage::UserRepository users(database);
        std::mutex storageMutex;

        ThreadPool threadPool;
        const unsigned int MAX_THREADS = std::thread::hardware_concurrency();

        threadPool.Initialize(MAX_THREADS > 0 ? MAX_THREADS : 4);
        smtp::QueueDispatcher queueDispatcher(
            configuration.Server().delivery,
            threadPool,
            users,
            mailMessages,
            messageRecipients,
            storageMutex);

        const std::string username = ReadEnv("SMTP_AUTH_USER");
        const std::string password = ReadEnv("SMTP_AUTH_PASSWORD");
        if (!username.empty())
        {
            authService.AddUser(username, password);
        }
        smtp::SmtpServerDependencies dependencies{
            socketsManager,
            threadPool,
            sessionHandler,
            authService,
            database,
            mailMessages,
            messageRecipients,
            storageMutex,
            queueDispatcher};

        smtp::SmtpServer server(configuration.Server(), dependencies);

        std::cout << "SMTP Server started with " << (MAX_THREADS > 0 ? MAX_THREADS : 4) << " threads\n";
        logger.LogProd("main", "SMTP Server successfully started.");

        server.Start();

        return 0;
    }
    catch (const std::exception &error)
    {
        std::cerr << "SMTP server startup failed: " << error.what() << '\n';
        logger.LogProd("main", std::string("Startup failed: ") + error.what());
        return 1;
    }
}
