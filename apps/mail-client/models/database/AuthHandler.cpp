#include "headers/database/AuthHandler.h"

#include <boost/json.hpp>

#include "headers/service/Service.h"
#include "headers/users/CurrentUser.h"

namespace json = boost::json;

AuthHandler::AuthHandler(QObject* parent)
    : QObject(parent)
{
}

bool AuthHandler::registerUser(const QString& username, const QString& email, const QString& password)
{
    try {
        const auto response = ISXService::Service::MailServerClient().Register(
            username.toStdString(), email.toStdString(), password.toStdString());

        if (!response.is_success() || !response.body.is_object()) {
            return false;
        }

        ISXCurrentUser::CurrentUser::GetInstance().Authorize(username, email, "");
        emit
        registrationSuccess();
        return true;
    } catch (const std::exception& exception) {
        ISXService::Service::Logger().Log(Logging::LogLevel::Error,
                                          std::string("AuthHandler::registerUser failed: ") + exception.what());
        return false;
    }
}

bool AuthHandler::loginUser(const QString& email, const QString& password)
{
    try {
        const auto response =
            ISXService::Service::MailServerClient().Login(email.toStdString(), password.toStdString());

        if (!response.is_success() || !response.body.is_object()) {
            return false;
        }

        const json::object& object = response.body.as_object();
        const json::value* user_value = object.if_contains("user");
        if (user_value == nullptr || !user_value->is_object()) {
            return false;
        }

        const json::object& user = user_value->as_object();
        const json::value* username_value = user.if_contains("username");
        const json::value* email_value = user.if_contains("email");
        if (username_value == nullptr || email_value == nullptr || !username_value->is_string() ||
            !email_value->is_string()) {
            return false;
        }

        ISXCurrentUser::CurrentUser::GetInstance().Authorize(
            QString::fromStdString(std::string(username_value->as_string().c_str())),
            QString::fromStdString(std::string(email_value->as_string().c_str())),
            "");
        emit
        loginSuccess();
        return true;
    } catch (const std::exception& exception) {
        ISXService::Service::Logger().Log(Logging::LogLevel::Error,
                                          std::string("AuthHandler::loginUser failed: ") + exception.what());
        return false;
    }
}
