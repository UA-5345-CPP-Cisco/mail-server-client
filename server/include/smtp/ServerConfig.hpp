#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace smtp {

    struct dbConfig {
        //
    };

struct TlsConfig {
    bool enabled{false};
    std::string certificatePath;
    std::string privateKeyPath;
};

struct ServerConfig {
    std::string host{"0.0.0.0"};
    std::uint16_t port{25};
    std::string serverName{"localhost"};
    std::size_t maxConnections{1024};
    TlsConfig tls;
};

}
