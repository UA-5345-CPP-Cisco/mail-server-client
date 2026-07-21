#pragma once

#include <optional>
#include <string>

#include "logger/Logger.h"

namespace ISXConfig {

    struct ClientConfig
    {
        Logging::LogLevel logLevel;
        std::string mailServerHost;
        std::string mailServerPort;
    };

    class IConfigReader
    {
    public:
        virtual ~IConfigReader() = default;
        virtual std::optional<ClientConfig> ReadConfig(const std::string& path) = 0;
    };

    class ClientConfigReader : public IConfigReader
    {
    public:
        std::optional<ClientConfig> ReadConfig(const std::string& path) override;
        static std::string ConfigPath();
    };

} // namespace ISXConfig
