#include<gtest/gtest.h>

#include "headers/database/DatabaseManager.h"

#include "../../../../libs/mail-storage/include/mail_storage/Database.h"
#include "../../../../libs/mail-storage/include/mail_storage/MigrationRunner.h"

class DatabaseManagerTests : public ::testing::Test {};

TEST_F(DatabaseManagerTests, WhenDatabasePathCalled_DoesReturnNonEmptyAbsolutePath) {
    std::filesystem::path return_path =ISXDatabaseManager::DatabaseManager::DatabasePath();
    EXPECT_TRUE(return_path.is_absolute());
    EXPECT_FALSE(return_path.empty());
}

TEST_F(DatabaseManagerTests, WhenDatabasePathCalled_DoesReturnCorrectFilename) {
    std::filesystem::path return_path =ISXDatabaseManager::DatabaseManager::DatabasePath();
    EXPECT_FALSE(return_path.empty());
    EXPECT_EQ(return_path.filename().string(),"qtapptestmail.sqlite");
}

TEST_F(DatabaseManagerTests, WhenMigrationsPathCalled_DoesReturnNonEmptyAbsolutePath) {
    std::filesystem::path return_path = ISXDatabaseManager::DatabaseManager::MigrationsPath();
    EXPECT_TRUE(return_path.is_absolute());
    EXPECT_FALSE(return_path.empty());
}

TEST_F(DatabaseManagerTests, WhenEnsureInitialized_DoesCreateDatabaseFileOnDisk) {
    std::filesystem::path old_path=ISXDatabaseManager::DatabaseManager::DatabasePath();
    std::filesystem::remove(old_path);
    ISXDatabaseManager::DatabaseManager::EnsureInitialized();
    std::filesystem::path return_path=ISXDatabaseManager::DatabaseManager::DatabasePath();
    EXPECT_TRUE(std::filesystem::exists(return_path));
    std::filesystem::remove(return_path);
}