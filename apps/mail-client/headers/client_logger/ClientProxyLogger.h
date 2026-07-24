#pragma once

#include "ClientConfigReader.h"
#include "logger/Logger.h"

namespace ISXClientLogger {

    class ILoggerProvider
    {
    public:
        virtual ~ILoggerProvider() = default;
        [[nodiscard]] virtual Logging::LogLevel MinimumLogLevel() const = 0;
    };

    class ClientLogger : public Logging::ILogger
    {
    public:
        ClientLogger(ILogger& logger, ILoggerProvider& provider);
        void Log(Logging::LogLevel level, std::string_view message) override;

    private:
        const ILoggerProvider& m_loggerProvider;
        ILogger& m_logger;
    };

    class ClientLoggerProvider : public ILoggerProvider
    {
    public:
        ClientLoggerProvider(ISXConfig::ClientConfig config);
        [[nodiscard]] Logging::LogLevel MinimumLogLevel() const override;

    private:
        ISXConfig::ClientConfig m_clientConfig;
    };
} // namespace ISXClientLogger