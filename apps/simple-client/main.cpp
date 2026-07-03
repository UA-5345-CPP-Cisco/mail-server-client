#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

#include "smtp_client/MailBuilder.h"
#include "smtp_client/SmtpClient.h"

namespace {

struct DemoOptions
{
  std::string host{"127.0.0.1"};
  std::uint16_t port{2525};
  std::string sender{"sender@example.test"};
  std::string recipient{"local@example.test"};
  std::string subject{"SMTP SDK demo"};
  std::string body{"Hello from simple_smtp_client"};
};

void PrintUsage(const char* executable)
{
  std::cout << "Usage: " << executable << " [host] [port] [sender] [recipient] [subject] [body]\n";
}

std::uint16_t ParsePort(const std::string& value)
{
  const unsigned long port = std::stoul(value);
  if (port == 0 || port > 65535)
  {
    throw std::runtime_error("Port must be between 1 and 65535");
  }

  return static_cast<std::uint16_t>(port);
}

DemoOptions ParseOptions(int argumentCount, char* arguments[])
{
  DemoOptions options;

  if (argumentCount > 1)
  {
    const std::string first = arguments[1];
    if (first == "-h" || first == "--help")
    {
      PrintUsage(arguments[0]);
      std::exit(0);
    }
    options.host = first;
  }

  if (argumentCount > 2)
  {
    options.port = ParsePort(arguments[2]);
  }
  if (argumentCount > 3)
  {
    options.sender = arguments[3];
  }
  if (argumentCount > 4)
  {
    options.recipient = arguments[4];
  }
  if (argumentCount > 5)
  {
    options.subject = arguments[5];
  }
  if (argumentCount > 6)
  {
    options.body = arguments[6];
  }

  return options;
}

} // namespace

int main(int argumentCount, char* arguments[])
{
  try
  {
    const DemoOptions options = ParseOptions(argumentCount, arguments);
    Smtp::SmtpClient client(options.host, options.port);

    std::cout << "Checking SMTP server at " << options.host << ':' << options.port << '\n';

    if (!client.CheckConnection())
    {
      std::cerr << "Connection check failed\n";
      return 1;
    }

    const Smtp::Mail mail = Smtp::MailBuilder()
                              .SetSender(options.sender)
                              .AddRecipients({options.recipient})
                              .SetSubject(options.subject)
                              .SetBody(options.body)
                              .Build();

    std::cout << "Sending message from " << options.sender << " to " << options.recipient << '\n';

    if (!client.SendMailMessage(mail))
    {
      std::cerr << "Message sending failed\n";
      return 1;
    }

    std::cout << "Message sent successfully\n";
    return 0;
  }
  catch (const std::exception& error)
  {
    std::cerr << "simple_smtp_client failed: " << error.what() << '\n';
    return 1;
  }
}
