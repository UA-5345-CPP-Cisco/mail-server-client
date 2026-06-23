#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

namespace smtp
{

	struct AuthRequest
	{
		std::string mechanism;
		std::string username;
		std::string secret;
	};

	struct AuthResult
	{
		bool accepted{false};
		std::string identity;
	};

	class IAuthService
	{
	public:
		virtual ~IAuthService() = default;

		virtual AuthResult Authenticate(const AuthRequest &request) = 0;
	};

	class AuthService final : public IAuthService
	{
	public:
		AuthService() = default;
		explicit AuthService(std::unordered_map<std::string, std::string> users);

		void AddUser(std::string username, std::string secret);
		AuthResult Authenticate(const AuthRequest &request) override;

	private:
		std::mutex mutex_;
		std::unordered_map<std::string, std::string> users_;
	};

}
