#include "smtp/AuthService.hpp"

#include <utility>

namespace smtp {

AuthService::AuthService(std::unordered_map<std::string, std::string> users)
	: users_(std::move(users))
{
}

void AuthService::AddUser(std::string username, std::string secret)
{
	std::lock_guard<std::mutex> lock(mutex_);
	users_[std::move(username)] = std::move(secret);
}

AuthResult AuthService::Authenticate(const AuthRequest& request)
{
	std::lock_guard<std::mutex> lock(mutex_);
	const auto user = users_.find(request.username);
	if (user == users_.end() || user->second != request.secret) {
		return {};
	}

	return AuthResult{true, request.username};
}

}
