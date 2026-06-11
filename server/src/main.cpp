#include "SMTP_Server.hpp"

#include "smtp/Logger.hpp"
#include "smtp/Services.hpp"
#include "smtp/SocketsManager.hpp"
#include "smtp/ThreadPool.hpp"

#include <cstdlib>
#include <string>

namespace {

std::string ReadEnv(const char* name, std::string fallback = {})
{
    if (const char* value = std::getenv(name)) {
        return value;
    }
    return fallback;
}

std::uint16_t ReadPort()
{
    const std::string value = ReadEnv("SMTP_PORT", "2525");
    return static_cast<std::uint16_t>(std::stoul(value));
}

bool ReadFlag(const char* name, bool fallback = false)
{
    const std::string value = ReadEnv(name);
    if (value.empty()) {
        return fallback;
    }
    return value == "1" || value == "true" || value == "TRUE" || value == "yes";
}

}

int main()
{
    smtp::ServerConfig config;
    config.host = ReadEnv("SMTP_HOST", "0.0.0.0");
    config.port = ReadPort();
    config.serverName = ReadEnv("SMTP_SERVER_NAME", "localhost");
    config.requireAuthentication = ReadFlag("SMTP_REQUIRE_AUTH");
    config.allowPlainAuthenticationWithoutTls = ReadFlag("SMTP_ALLOW_PLAIN_AUTH_WITHOUT_TLS", true);

    const std::string certificate = ReadEnv("SMTP_TLS_CERT");
    const std::string privateKey = ReadEnv("SMTP_TLS_KEY");
    if (!certificate.empty() && !privateKey.empty()) {
        config.tls.enabled = true;
        config.tls.certificatePath = certificate;
        config.tls.privateKeyPath = privateKey;
    }

    smtp::BoostSocketsManager socketsManager;
    smtp::ThreadPool threadPool;
    smtp::SmtpSessionHandler sessionHandler;
    smtp::AuthService authService;
    smtp::dbSQLite mailStorage({ReadEnv("SMTP_STORAGE_PATH", "mail-storage")});
    smtp::CacheService cacheService;
    smtp::DeliveryService deliveryService;
    smtp::LookupService lookupService;
    smtp::Logger logger;

    const std::string username = ReadEnv("SMTP_AUTH_USER");
    const std::string password = ReadEnv("SMTP_AUTH_PASSWORD");
    if (!username.empty()) {
        authService.AddUser(username, password);
    }

    smtp::SmtpServerDependencies dependencies{
        socketsManager,
        threadPool,
        sessionHandler,
        authService,
        mailStorage,
        cacheService,
        deliveryService,
        lookupService,
        logger
    };

    smtp::SmtpServer server(config, dependencies);
    server.Start();
    return 0;
}
