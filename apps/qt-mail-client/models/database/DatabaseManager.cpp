#include "headers/database/DatabaseManager.h"

#include <cstdlib>
#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

#include "../../../../libs/storage/include/storage/Database.h"
#include "../../../../libs/storage/include/storage/MigrationRunner.h"

namespace ISXDatabaseManager
{

void DatabaseManager::EnsureInitialized()
{
	Storage::Database database(DatabasePath());
	Storage::MigrationRunner runner(database, MigrationsPath());
	runner.Run();
}

std::filesystem::path DatabaseManager::DatabasePath()
{
	return ResolveAppDataDirectory() / "qtapptestmail.sqlite";
}

std::filesystem::path DatabaseManager::MigrationsPath()
{
	const std::filesystem::path relative_path = std::filesystem::path("libs") / "storage" / "migrations";
	const std::filesystem::path current_path = std::filesystem::current_path() / relative_path;

	if (std::filesystem::exists(current_path))
	{
		return current_path;
	}

	std::filesystem::path probe = std::filesystem::path(QCoreApplication::applicationDirPath().toStdString());

	for (int depth = 0; depth < 6; ++depth)
	{
		const std::filesystem::path candidate = probe / relative_path;
		if (std::filesystem::exists(candidate))
		{
			return candidate;
		}

		if (!probe.has_parent_path())
		{
			break;
		}

		probe = probe.parent_path();
	}

	return current_path;
}

std::filesystem::path DatabaseManager::ResolveAppDataDirectory()
{
	QString app_data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	if (app_data_dir.isEmpty())
	{
		app_data_dir = QCoreApplication::applicationDirPath() + "/qtapptestmail";
	}

	QDir dir;
	if (!dir.exists(app_data_dir))
	{
		dir.mkpath(app_data_dir);
	}

	return std::filesystem::path(app_data_dir.toStdString());
}

}
