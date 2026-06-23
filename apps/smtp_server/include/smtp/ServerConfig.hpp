#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace smtp
{

    struct DbConfig
    {
        std::string storagePath{"mail-storage"};
        std::string migrationsPath{"libs/storage/migrations"};
    };

    struct TlsConfig
    {
        bool enabled{false};
        std::string certificatePath;
        std::string privateKeyPath;
    };

    struct DeliveryConfig
    {
        std::size_t batchSize{32};
        std::uint32_t pollIntervalMilliseconds{100};
    };

    struct ServerConfig
    {
        std::string host{"0.0.0.0"};
        std::uint16_t port{25};
        std::string serverName{"localhost"};
        std::size_t maxConnections{1024};
        std::size_t maxMessageSizeBytes{10 * 1024 * 1024};
        bool requireAuthentication{false};
        bool allowPlainAuthenticationWithoutTls{false};
        TlsConfig tls;
        DeliveryConfig delivery;
    };

}
