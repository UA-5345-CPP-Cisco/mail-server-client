#include "headers/database/DatabaseManager.h"

#include <cstdlib>
#include <string>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

#include "mail_storage/Database.h"
#include "headers/service/Service.h"
#include "mail_storage/MigrationRunner.h"

namespace ISXDatabaseManager
{

void DatabaseManager::EnsureInitialized()
{
	 ISXService::Service::Logger().Log(Logging::LogLevel::Info, std::string("DatabaseManager::EnsureInitialized: database_path=") + DatabasePath().string());
	Storage::Database database(DatabasePath());
	 ISXService::Service::Logger().Log(Logging::LogLevel::Info, std::string("DatabaseManager::EnsureInitialized: migrations_path=") + MigrationsPath().string());
	Storage::MigrationRunner runner(database, MigrationsPath());
	runner.Run();
	 ISXService::Service::Logger().Log(Logging::LogLevel::Info, "DatabaseManager::EnsureInitialized: migrations applied");
}

std::filesystem::path DatabaseManager::DatabasePath()
{
	auto path = ResolveAppDataDirectory() / "qtapptestmail.sqlite";
	 ISXService::Service::Logger().Log(Logging::LogLevel::Debug, std::string("DatabaseManager::DatabasePath: ") + path.string());
	return path;
}

std::filesystem::path DatabaseManager::MigrationsPath()
{
	const std::filesystem::path relative_path = std::filesystem::path("libs") / "mail-storage" / "migrations";
	const std::filesystem::path current_path = std::filesystem::current_path() / relative_path;

	if (std::filesystem::exists(current_path))
	{
		 ISXService::Service::Logger().Log(Logging::LogLevel::Debug, (std::string("DatabaseManager::MigrationsPath: using current_path=") + current_path.string()));
		return current_path;
	}

	std::filesystem::path probe = std::filesystem::path(QCoreApplication::applicationDirPath().toStdString());

	for (int depth = 0; depth < 6; ++depth)
	{
		const std::filesystem::path candidate = probe / relative_path;
		 ISXService::Service::Logger().Log(Logging::LogLevel::Debug, (std::string("DatabaseManager::MigrationsPath: probing candidate=") + candidate.string()));
		if (std::filesystem::exists(candidate))
		{
			 ISXService::Service::Logger().Log(Logging::LogLevel::Debug, (std::string("DatabaseManager::MigrationsPath: found candidate=") + candidate.string()));
			return candidate;
		}

		if (!probe.has_parent_path())
		{
			break;
		}

		probe = probe.parent_path();
	}

	 ISXService::Service::Logger().Log(Logging::LogLevel::Warning, (std::string("DatabaseManager::MigrationsPath: falling back to current_path=") + current_path.string()));
	return current_path;
}

std::filesystem::path DatabaseManager::ResolveAppDataDirectory()
{
	QString app_data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	if (app_data_dir.isEmpty())
	{
		app_data_dir = QCoreApplication::applicationDirPath() + "/qtapptestmail";
		 ISXService::Service::Logger().Log(Logging::LogLevel::Warning, std::string("DatabaseManager::ResolveAppDataDirectory: writableLocation was empty, using ") + app_data_dir.toStdString());
	}
	else
	{
		 ISXService::Service::Logger().Log(Logging::LogLevel::Debug, std::string("DatabaseManager::ResolveAppDataDirectory: using ") + app_data_dir.toStdString());
	}

	QDir dir;
	if (!dir.exists(app_data_dir))
	{
		dir.mkpath(app_data_dir);
		 ISXService::Service::Logger().Log(Logging::LogLevel::Info, std::string("DatabaseManager::ResolveAppDataDirectory: created directory ") + app_data_dir.toStdString());
	}

	return std::filesystem::path(app_data_dir.toStdString());
}

}
