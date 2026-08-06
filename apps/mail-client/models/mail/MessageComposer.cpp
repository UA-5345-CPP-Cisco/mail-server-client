#include "headers/mail/MessageComposer.h"

#include <optional>


#include "headers/service/Service.h"

namespace ISXMail {

    namespace {

        std::optional<std::string> ToOptionalString(const QString& text)
        {
            const QString trimmed = text.trimmed();

            if (trimmed.isEmpty()) {
                return std::nullopt;
            }

            return trimmed.toStdString();
        }

    } // namespace

    MessageComposer::MessageComposer(QObject* parent)
        : QObject(parent)
    {
        ISXService::Service::Logger().Log(Logging::LogLevel::Info,
                                          "MessageComposer: initialized without DB");
    }

    bool MessageComposer::SendMailMessage(const QString& sender_name,
                                          const QString& sender_email,
                                          const QString& recipient_email,
                                          const QString& subject,
                                          const QString& body,
                                          bool is_inbox)
    {
        Q_UNUSED(sender_name);
        Q_UNUSED(is_inbox);

        if (recipient_email.trimmed().isEmpty() || body.trimmed().isEmpty()) {
            ISXService::Service::Logger().Log(
                Logging::LogLevel::Warning,
                "MessageComposer::SendMailMessage: validation failed - recipient or body empty");
            return false;
        }

        ISXService::Service::Logger().Log(Logging::LogLevel::Info,
                                          (std::string("MessageComposer::SendMailMessage: sending to ") +
                                           recipient_email.toStdString() +
                                           " subject_len=" + std::to_string(subject.size())));

        try {
            const auto response = ISXService::Service::MailServerClient().SendMail(
                sender_email.toStdString(), {recipient_email.toStdString()}, subject.toStdString(), body.toStdString());
            if (!response.is_success()) {
                ISXService::Service::Logger().Log(
                    Logging::LogLevel::Warning, "MessageComposer::SendMailMessage: mail server rejected send request");
                return false;
            }

            return true;
        } catch (const std::exception& exception) {
            ISXService::Service::Logger().Log(
                Logging::LogLevel::Error, std::string("MessageComposer::SendMailMessage failed: ") + exception.what());
            return false;
        }
    }

    bool MessageComposer::SaveDraft(const QString& sender_name,
                                    const QString& sender_email,
                                    const QString& recipient_email,
                                    const QString& subject,
                                    const QString& body)
    {
        Q_UNUSED(sender_name);

        if (recipient_email.trimmed().isEmpty() && subject.trimmed().isEmpty() && body.trimmed().isEmpty()) {
            return false;
        }

        // Local draft storage is currently disabled since we removed the DB.
        return false;
    }

} // namespace ISXMail
