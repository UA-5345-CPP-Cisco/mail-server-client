#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace smtp {

// Placeholder for database settings used by the future storage component.
struct dbConfig {
    //
};

// TLS settings used by the sockets manager when encrypted SMTP is enabled.
struct TlsConfig {
    bool enabled{false};
    std::string certificatePath;
    std::string privateKeyPath;
};

// Runtime settings that describe how the SMTP server should listen and identify itself.
struct ServerConfig {
    std::string host{"0.0.0.0"};
    std::uint16_t port{25};
    std::string serverName{"localhost"};
    std::size_t maxConnections{1024};
    TlsConfig tls;
};

}
