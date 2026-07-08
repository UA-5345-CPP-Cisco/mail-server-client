#include <gtest/gtest.h>
#include <QTemporaryFile>
#include "users/AccountListModel.h"
#include "users/CurrentUser.h"
#include "database/DatabaseManager.h"
#include "../../../../libs/mail-storage/include/mail_storage/UserRepository.h"

class AccountListModelTests : public ::testing::Test {
protected:
    void SetUp() override {
        std::filesystem::remove(ISXDatabaseManager::DatabaseManager::DatabasePath());
        ISXDatabaseManager::DatabaseManager::EnsureInitialized();
        ISXCurrentUser::CurrentUser::GetInstance().Logout();
    }

    void TearDown() override {
        ISXCurrentUser::CurrentUser::GetInstance().Logout();
    }
};

TEST_F(AccountListModelTests, CanChangeRowCount) {
    ISXMail::AccountListModel model;
    const ISXMail::AccountData temp_data0, temp_data1;
    int row_count_now = model.rowCount();
    model.AddData(temp_data0);
    EXPECT_EQ(model.rowCount(), row_count_now + 1);
    model.AddData(temp_data1);
    EXPECT_EQ(model.rowCount(), row_count_now + 2);
    EXPECT_TRUE(model.RemoveAccount(1));
    EXPECT_EQ(model.rowCount(), row_count_now + 1);
}

TEST_F(AccountListModelTests, DataParseCheck) {
    ISXMail::AccountListModel model;
    const ISXMail::AccountData temp_data{.account_name = "temp_role"};
    model.AddData(temp_data);
    QModelIndex index = model.index(0, 0);
    EXPECT_EQ(model.data(index, ISXMail::AccountNameRole).toString(), "temp_role");
    QModelIndex bad_index = model.index(999, 0);
    EXPECT_TRUE(model.data(bad_index, ISXMail::AccountNameRole).isNull());
}

TEST_F(AccountListModelTests, RoleNameCheck) {
    ISXMail::AccountListModel model;
    auto const role = model.roleNames();
    EXPECT_EQ(role.size(), 6);
    EXPECT_EQ(role.value(ISXMail::AccountNameRole), QByteArray("accountName"));
}

TEST_F(AccountListModelTests, SetDataTest) {
    ISXMail::AccountListModel model;
    const QVariant empty_value;
    int role = ISXMail::AccountNameRole;
    QModelIndex bad_index = model.index(999, 0);
    QModelIndex index = model.index(0, 0);
    EXPECT_FALSE(model.setData(bad_index, empty_value, role));
    EXPECT_FALSE(model.setData(index, empty_value, -1));
    const ISXMail::AccountData temp_data{.account_name = "OLD_NAME"};
    model.AddData(temp_data);
    QModelIndex valid_index = model.index(0, 0);
    model.setData(valid_index, "NEW_NAME", ISXMail::AccountNameRole);
    EXPECT_EQ(model.data(valid_index, ISXMail::AccountNameRole).toString(), "NEW_NAME");
}

TEST_F(AccountListModelTests, AddAcountTestNonActive) {
    ISXMail::AccountListModel model;
    int old_row_count = model.rowCount();
    model.AddAccount("bob", "bob", "bob", "bob", "bob", false);
    EXPECT_EQ(model.rowCount(), old_row_count + 1);
    QModelIndex index = model.index(0, 0);
    EXPECT_EQ("bob", model.data(index, ISXMail::AccountNameRole).toString());
}

TEST_F(AccountListModelTests, AddAcountTestActive) {
    ISXMail::AccountListModel model;
    model.AddAccount("bob", "bob", "bob", "bob", "bob", true);
    auto &user = ISXCurrentUser::CurrentUser::GetInstance();
    EXPECT_TRUE(user.is_authorized());
    EXPECT_EQ(user.username(), "bob");
}

TEST_F(AccountListModelTests, ActiveAccount) {
    ISXMail::AccountListModel model;
    model.AddAccount("bob", "bob", "bob", "bob", "bob", false);
    model.SetActiveAccount(0);
    auto &user = ISXCurrentUser::CurrentUser::GetInstance();
    EXPECT_TRUE(user.is_authorized());
    EXPECT_EQ(user.email(), "bob");
}

TEST_F(AccountListModelTests, TestActiveAccountRow) {
    ISXMail::AccountListModel model;
    model.AddAccount("bob", "bob", "bob", "bob", "bob", false);
    model.SetActiveAccount(0);
    int res = model.ActiveAccountRow();
    EXPECT_NE(res, -1);
}

TEST_F(AccountListModelTests, LoadFromDatabaseActivatesFirstUserIfNoneActive) {
    Storage::Database db(ISXDatabaseManager::DatabaseManager::DatabasePath());
    Storage::UserRepository repo(db);
    std::int64_t id1 = repo.CreateUser("bob", "bob", "bob");
    repo.UpdateStatus(id1, Storage::UserStatus::Disabled);
    std::int64_t id2 = repo.CreateUser("Tom", "Tom", "Tom");
    repo.UpdateStatus(id2, Storage::UserStatus::Disabled);
    ISXMail::AccountListModel model;
    EXPECT_EQ(model.rowCount(), 2);
    QModelIndex first_user_index = model.index(0, 0);
    EXPECT_TRUE(model.data(first_user_index, ISXMail::IsActiveRole).toBool());
    auto& current_user = ISXCurrentUser::CurrentUser::GetInstance();
    EXPECT_TRUE(current_user.is_authorized());
    EXPECT_EQ(current_user.username(), "bob");
}

TEST_F(AccountListModelTests, LoadFromDatabaseRestoresAlreadyActiveUser) {
    Storage::Database db(ISXDatabaseManager::DatabaseManager::DatabasePath());
    Storage::UserRepository repo(db);
    std::int64_t id_alice = repo.CreateUser("Tom", "Tom", "Tom");
    repo.UpdateStatus(id_alice, Storage::UserStatus::Disabled);
    std::int64_t id_bob = repo.CreateUser("bob", "bob", "bob");
    repo.UpdateStatus(id_bob, Storage::UserStatus::Active);
    ISXMail::AccountListModel model;
    EXPECT_GE(model.rowCount(), 2);
    int active_row = model.ActiveAccountRow();
    EXPECT_NE(active_row, -1);
    QModelIndex bob_index = model.index(active_row, 0);
    EXPECT_EQ(model.data(bob_index, ISXMail::AccountNameRole).toString(), "bob");
    auto& current_user = ISXCurrentUser::CurrentUser::GetInstance();
    EXPECT_TRUE(current_user.is_authorized());
    EXPECT_EQ(current_user.username(), "bob");
}