#pragma once

#include "headers/client_logger/ClientProxyLogger.h"
#include "logger/Logger.h"
#include "mail_server_sdk/MailServerClient.h"

namespace ISXService {

    class Service
    {
    public:
        static void Initialize(ISXClientLogger::ClientLogger& logger);
        static void Initialize(ISXClientLogger::ClientLogger& logger, ISXMailServerSDK::MailServerClient& mail_client);
        static ISXClientLogger::ClientLogger& Logger();
        static ISXMailServerSDK::MailServerClient& MailServerClient();

    private:
        inline static ISXClientLogger::ClientLogger* m_logger = nullptr;
        inline static ISXMailServerSDK::MailServerClient* m_mail_client = nullptr;
    };

} // namespace ISXService
