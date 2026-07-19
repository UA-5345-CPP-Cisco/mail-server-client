#pragma once

#include "headers/client_logger/ClientProxyLogger.h"
#include "logger/Logger.h"

namespace ISXService {

    class Service
    {
    public:
        static void Initialize(ISXClientLogger::ClientLogger& logger);
        static ISXClientLogger::ClientLogger& Logger();

    private:
        inline static ISXClientLogger::ClientLogger* m_logger = nullptr;
    };

} // namespace ISXService
