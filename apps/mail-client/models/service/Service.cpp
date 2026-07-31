#include "headers/service/Service.h"

namespace ISXService {

    void Service::Initialize(ISXClientLogger::ClientLogger& logger)
    {
        m_logger = &logger;
    }

    void Service::Initialize(ISXClientLogger::ClientLogger& logger, ISXMailServerSDK::MailServerClient& mail_client)
    {
        m_logger = &logger;
        m_mail_client = &mail_client;
    }

    ISXClientLogger::ClientLogger& Service::Logger()
    {
        return *m_logger;
    }

    ISXMailServerSDK::MailServerClient& Service::MailServerClient()
    {
        return *m_mail_client;
    }

} // namespace ISXService
