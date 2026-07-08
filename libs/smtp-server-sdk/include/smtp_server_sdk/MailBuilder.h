#pragma once

#include <string>
#include <vector>

namespace Smtp {

struct Mail
{
  std::vector<std::string> recipients;
  std::string subject;
  std::string sender;
  std::string body;
};

class MailBuilder
{
  public:
  MailBuilder& AddRecipients(const std::vector<std::string>& recipients);

  MailBuilder& SetSender(const std::string& sender);

  MailBuilder& SetSubject(const std::string& subject);

  MailBuilder& SetBody(const std::string& body);

  Mail Build();

  private:
  Mail m_mail;
};

} // namespace Smtp
