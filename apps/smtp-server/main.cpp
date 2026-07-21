#include <cstdlib>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>

#include "SMTP_Server.hpp"
#include "logger/Logger.h"
#include "mail_storage/Database.h"
#include "mail_storage/MailMessageRepository.h"
#include "mail_storage/MessageRecipientRepository.h"
#include "mail_storage/MigrationRunner.h"
#include "mail_storage/UserRepository.h"
#include "smtp/AuthService.hpp"
#include "smtp/Configuration.hpp"
#include "smtp/QueueDispatcher.hpp"
#include "smtp/SocketsManager.hpp"
#include "thread_pool/ThreadPool.h"

namespace {

std::string ReadEnv(const char* name, std::string fallback = {})
{
  if (const char* value = std::getenv(name))
  {
    return value;
  }
  return fallback;
}

} // namespace

int main(int argumentCount, char* arguments[])
{
  try
  {
    const std::string configurationPath = argumentCount > 1 ? arguments[1] : "config/smtp_server.json";
    const smtp::Configuration configuration(configurationPath);

    smtp::BoostSocketsManager socketsManager;
    smtp::SmtpSessionHandler sessionHandler;
    smtp::AuthService authService;
    Storage::Database database(configuration.Database().storagePath);
    Storage::MigrationRunner migrationRunner(database, configuration.Database().migrationsPath);
    migrationRunner.Run();
    Storage::MailMessageRepository mailMessages(database);
    Storage::MessageRecipientRepository messageRecipients(database);
    Storage::UserRepository users(database);
    std::mutex storageMutex;
    Logging::Logger logger;
    Concurrency::ThreadPool threadPool;
    smtp::QueueDispatcher queueDispatcher(
      configuration.Server().delivery, threadPool, users, mailMessages, messageRecipients, storageMutex, logger);

    const std::string username = ReadEnv("SMTP_AUTH_USER");
    const std::string password = ReadEnv("SMTP_AUTH_PASSWORD");
    if (!username.empty())
    {
      authService.AddUser(username, password);
    }

    smtp::SmtpServerDependencies dependencies{socketsManager,
                                              threadPool,
                                              sessionHandler,
                                              authService,
                                              database,
                                              mailMessages,
                                              messageRecipients,
                                              storageMutex,
                                              queueDispatcher,
                                              logger};

    smtp::SmtpServer server(configuration.Server(), dependencies);
    server.Start();
    return 0;
  }
  catch (const std::exception& error)
  {
    std::cerr << "SMTP server startup failed: " << error.what() << '\n';
    return 1;
  }
}
