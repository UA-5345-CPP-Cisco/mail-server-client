#include<gtest/gtest.h>
#include <memory>

#include "database/RegistrationHandler.h"
#include "mail_storage/UserRepository.h"
#include "database/DatabaseManager.h"

class RegistrationHandlerTests : public ::testing::Test {
    protected:
    std::unique_ptr<RegistrationHandler> registrationHandler;
    std::unique_ptr<Storage::Database> db;

    void SetUp() override{
        ISXDatabaseManager::DatabaseManager::EnsureInitialized();
        this->db = std::make_unique<Storage::Database>(ISXDatabaseManager::DatabaseManager::DatabasePath());
        this->db->Execute("DELETE FROM users;");
        this->registrationHandler = std::make_unique<RegistrationHandler>(*this->db);
    }

    void TearDown() override {
        this->db.reset();
        this->registrationHandler.reset();
    }
};

TEST_F(RegistrationHandlerTests,WhenregisterUser_RegistrationHandlerDoesReturnFalse) {
    const QString username="";
    const QString email="";
    const QString password="";
    EXPECT_FALSE(registrationHandler->registerUser(username, "Some text", "Some text"));
    EXPECT_FALSE(registrationHandler->registerUser("Some text", email, "Some text"));
    EXPECT_FALSE(registrationHandler->registerUser("Some text", "Some text", password));
}

TEST_F(RegistrationHandlerTests,WhenregisterUser_RegistrationHandlerDoesReturnTrue) {
    const QString username="Bob";
    const QString email="bob@gmail";
    const QString password="Some text";
    EXPECT_TRUE(this->registrationHandler->registerUser(username, email, password));
    Storage::UserRepository repo(*this->db);
    auto user=repo.FindByEmail(email.toStdString());
    EXPECT_TRUE(user.has_value());
    EXPECT_EQ(user->username,username);
}

TEST_F(RegistrationHandlerTests, WhenRegisterDuplicateUser_ReturnsFalse) {
    const QString username = "bob";
    const QString email = "bob@gmail";
    const QString password = "bob_top";
    EXPECT_TRUE(this->registrationHandler->registerUser(username, email, password));
    EXPECT_FALSE(this->registrationHandler->registerUser("AnotherName", email, "newpassword"));
}


