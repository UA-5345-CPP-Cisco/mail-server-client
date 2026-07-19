#include<gtest/gtest.h>
#include <memory>

#include "database/AuthHandler.h"
#include "mail_storage/UserRepository.h"
#include "database/DatabaseManager.h"

class AuthHandlerTests : public ::testing::Test {
    protected:
    std::unique_ptr<AuthHandler> AuthHandler;
    std::unique_ptr<Storage::Database> db;

    void SetUp() override{
        ISXDatabaseManager::DatabaseManager::EnsureInitialized();
        this->db = std::make_unique<Storage::Database>(ISXDatabaseManager::DatabaseManager::DatabasePath());
        this->db->Execute("DELETE FROM users;");
        this->AuthHandler = std::make_unique<AuthHandler>(*this->db);
    }

    void TearDown() override {
        this->db.reset();
        this->AuthHandler.reset();
    }
};

TEST_F(AuthHandlerTests,WhenregisterUser_RegistrationHandlerDoesReturnFalse) {
    const QString username="";
    const QString email="";
    const QString password="";
    EXPECT_FALSE(AuthHandler->RegisterUser(username, "Some text", "Some text"));
    EXPECT_FALSE(AuthHandler->RegisterUser("Some text", email, "Some text"));
    EXPECT_FALSE(AuthHandler->RegisterUser("Some text", "Some text", password));
}

TEST_F(AuthHandlerTests,WhenregisterUser_RegistrationHandlerDoesReturnTrue) {
    const QString username="Bob";
    const QString email="bob@gmail";
    const QString password="Some text";
    EXPECT_TRUE(this->AuthHandler->RegisterUser(username, email, password));
    Storage::UserRepository repo(*this->db);
    auto user=repo.FindByEmail(email.toStdString());
    EXPECT_TRUE(user.has_value());
    EXPECT_EQ(user->username,username);
}

TEST_F(AuthHandlerTests, WhenRegisterDuplicateUser_ReturnsFalse) {
    const QString username = "bob";
    const QString email = "bob@gmail";
    const QString password = "bob_top";
    EXPECT_TRUE(this->AuthHandler->RegisterUser(username, email, password));
    EXPECT_FALSE(this->AuthHandler->RegisterUser("AnotherName", email, "newpassword"));
}


