#pragma once

#include <filesystem>
#include <string>

namespace ISXDatabaseManager
{

class DatabaseManager
{
public:
	static void EnsureInitialized();
	static std::filesystem::path DatabasePath();
	static std::filesystem::path MigrationsPath();

private:
	static std::filesystem::path ResolveAppDataDirectory();
};

}
