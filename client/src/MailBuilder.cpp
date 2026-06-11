#include "MailBuilder.hpp"
#include <stdexcept>

namespace ISXSmtpClient {
    MailBuilder &MailBuilder::AddRecipient(const std::vector<std::string> &recipients) {
        m_mail.recipients = recipients;
        return *this;
    }

    MailBuilder &MailBuilder::SetSender(const std::string &sender) {
        m_mail.sender = sender;
        return *this;
    }

    MailBuilder &MailBuilder::SetSubject(const std::string &subject) {
        m_mail.subject = subject;
        return *this;
    }

    MailBuilder &MailBuilder::SetBody(const std::string &body) {
        m_mail.body = body;
        return *this;
    }

    Mail MailBuilder::Build() {
        if (m_mail.sender.empty()) {
            throw std::runtime_error("Sender is required");
        }
        if (m_mail.recipients.empty()) {
            throw std::runtime_error("At least one recipient is required");
        }
        return m_mail;
    }
}