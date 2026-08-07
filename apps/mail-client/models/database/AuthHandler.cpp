#include "headers/database/AuthHandler.h"

#include <boost/json.hpp>

#include "headers/service/Service.h"
#include "headers/users/CurrentUser.h"

namespace json = boost::json;

AuthHandler::AuthHandler(QObject* parent) : QObject(parent) {}

AuthHandler::AuthResult AuthHandler::RegisterUser(const QString& username, const QString& email, const QString& password)
{
    ISXService::Service::Logger().Log(Logging::LogLevel::Info, "Registration attempt started for email: " + email.toStdString());
    try 
    {
        const auto response = ISXService::Service::MailServerClient().Register(
            username.toStdString(), email.toStdString(), password.toStdString());

        if (!response.is_success())
        {
            if (response.body.is_object())
            {
                const auto& object = response.body.as_object();
                const auto* error = object.if_contains("error");

                if (error != nullptr && error->is_string())
                {
                    const std::string message = error->as_string().c_str();

                    if (message.find("UNIQUE constraint") != std::string::npos)
                    {
                        ISXService::Service::Logger().Log(Logging::LogLevel::Warning, "Registration failed: User already exists (" + email.toStdString() + ")");
                        return AuthResult::UserAlreadyExists;
                    }
                }   
            }
            ISXService::Service::Logger().Log(Logging::LogLevel::Error, "Registration failed with server error code: " + std::to_string(response.status_code));
            return AuthResult::InternalError;
        }

        ISXCurrentUser::CurrentUser::GetInstance().Authorize(username, email, "");
        emit registrationSuccess();
        ISXService::Service::Logger().Log(Logging::LogLevel::Info, "Registration successful for email: " + email.toStdString());
        return AuthResult::Success;
    } 
    catch (const std::exception& exception) 
    {
        ISXService::Service::Logger().Log(Logging::LogLevel::Error,
                                          std::string("AuthHandler::RegisterUser failed: ") + exception.what());
        return AuthResult::InternalError;
    }
}

AuthHandler::AuthResult AuthHandler::LoginUser(const QString& email, const QString& password)
{
    ISXService::Service::Logger().Log(Logging::LogLevel::Info, 
                                      "Login attempt started for email: " + email.toStdString());
    try 
    {
        const auto response = ISXService::Service::MailServerClient().Login(email.toStdString(), password.toStdString());

        if (!response.is_success())
        {
            ISXService::Service::Logger().Log(Logging::LogLevel::Error, "Login failed with server error code: " + std::to_string(response.status_code));
            return AuthResult::WrongPasswordOREmail;
        }

        if (!response.body.is_object())
        {
            ISXService::Service::Logger().Log(Logging::LogLevel::Error, "Login failed: Response body is not a JSON object");
            return AuthResult::InternalError;
        }

        const json::object& object = response.body.as_object();
        const json::value* user_value = object.if_contains("user");
        if (user_value == nullptr || !user_value->is_object()) 
        {
            ISXService::Service::Logger().Log(Logging::LogLevel::Error, "Login failed: Missing or invalid 'user' object in JSON");
            return AuthResult::InternalError;
        }

        const json::object& user = user_value->as_object();
        const json::value* username_value = user.if_contains("username");
        const json::value* email_value = user.if_contains("email");
        if (username_value == nullptr || email_value == nullptr || !username_value->is_string() || !email_value->is_string()) 
        {
            ISXService::Service::Logger().Log(Logging::LogLevel::Error, "Login failed: Missing username or email in response payload");
            return AuthResult::InternalError;
        }

        ISXCurrentUser::CurrentUser::GetInstance().Authorize(
            QString::fromStdString(std::string(username_value->as_string().c_str())),
            QString::fromStdString(std::string(email_value->as_string().c_str())),
            "");
        emit loginSuccess();

        ISXService::Service::Logger().Log(Logging::LogLevel::Info, "Login successful for email: " + email.toStdString());

        return AuthResult::Success;
    } 
    catch (const std::exception& exception) 
    {
        ISXService::Service::Logger().Log(Logging::LogLevel::Error,
                                          std::string("AuthHandler::LoginUser failed: ") + exception.what());
        return AuthResult::InternalError;
    }
}