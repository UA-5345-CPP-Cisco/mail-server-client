#include <gtest/gtest.h>

#include <QString>
#include <memory>
#include <QFile>

#include "headers/mail/EmailListModel.h"

using namespace ISXMail;


class EmailListModelTest : public ::testing::Test {
protected:
    std::unique_ptr<EmailListModel> model;
    const QString test_bd = "test_mail_database.sqlite";

    void SetUp() override {
        model = std::make_unique<EmailListModel>();
    }

    void TearDown() override {
        model.reset();
        if (QFile::exists(test_bd)) {
            QFile::remove(test_bd);
        }
    }
};

TEST_F(EmailListModelTest, RowCountTestAddDel) {
    int start_row_count = model->rowCount();
    const EmailData item{};
    EXPECT_EQ(start_row_count, model->rowCount());
    model->AddData(item);
    EXPECT_EQ(start_row_count + 1, model->rowCount());
    QModelIndex index = model->index(0, 0);
    model->RemoveData(index.row());
    EXPECT_EQ(start_row_count, model->rowCount());
    EXPECT_EQ(model->rowCount(model->index(0, 0)), 0);
}

TEST_F(EmailListModelTest, DataTest) {
    EXPECT_TRUE(model->data(QModelIndex(), ThemeRole).isNull());
    QModelIndex bad_index = model->index(6767, 0);
    EXPECT_TRUE(model->data(bad_index, ThemeRole).isNull());
    EmailData item;
    item.id = -1;
    item.theme = "Test Theme";
    item.is_inbox = true;
    item.is_starred = false;
    model->AddData(item);
    QModelIndex validIndex = model->index(0, 0);
    EXPECT_EQ(model->data(validIndex, ThemeRole).toString().toStdString(), "Test Theme");
    EXPECT_TRUE(model->data(validIndex, InboxRole).toBool());
    EXPECT_FALSE(model->data(validIndex, StarredRole).toBool());
    EXPECT_TRUE(model->data(validIndex, Qt::DisplayRole).isNull());
}

TEST_F(EmailListModelTest, RoleNamesTest) {
    const auto roles = model->roleNames();
    EXPECT_EQ(roles.size(), 10);
    EXPECT_EQ(roles[InboxRole], QByteArray("emailsInbox"));
    EXPECT_EQ(roles[ThemeRole], QByteArray("emailsTheme"));
    EXPECT_EQ(roles[TimeRole], QByteArray("emailsTime"));
}

TEST_F(EmailListModelTest, RemoveDataOutOfBoundsTest) {
    model->RemoveData(0);
    EXPECT_EQ(model->rowCount(), 0);
    model->AddData(EmailData{});
    EXPECT_EQ(model->rowCount(), 1);
    model->RemoveData(-1);
    EXPECT_EQ(model->rowCount(), 1);
    model->RemoveData(6767);
    EXPECT_EQ(model->rowCount(), 1);
}

TEST_F(EmailListModelTest, DelEmailTest) {
    const EmailData item_false{.id = 67};
    model->AddData(item_false);
    QModelIndex index_false = model->index(0, 0);
    const EmailData item{};
    EXPECT_FALSE(model->DeleteEmail(index_false.row()));
    model->AddData(item);
    QModelIndex index = model->index(0, 0);
    model->RemoveData(index.row());
    EXPECT_EQ(model->rowCount(), 1);
}

TEST_F(EmailListModelTest, DeleteEmailTest) {
    EXPECT_FALSE(model->DeleteEmail(-1));
    EXPECT_FALSE(model->DeleteEmail(0));
    EmailData item_false;
    item_false.id = 67;
    model->AddData(item_false);
    EXPECT_FALSE(model->DeleteEmail(0));
    EXPECT_EQ(model->rowCount(), 1);
    EmailData item_true;
    item_true.id = -1;
    model->AddData(item_true);
    EXPECT_TRUE(model->DeleteEmail(0));
    EXPECT_EQ(model->rowCount(), 1);
}

TEST_F(EmailListModelTest, MakePreviewLogicTest) {
    QString shortContent = "Short text";
    model->AddData(false, false, false, "Theme", "Name", "To", shortContent, "", true);
    QModelIndex index0 = model->index(0, 0);
    EXPECT_EQ(model->data(index0, PreviewRole).toString().toStdString(), "Short text");
    QString contentWithSpace = "1234567890 1234567890 1234567890 long text";
    model->AddData(false, false, false, "Theme", "Name", "To", contentWithSpace, "", true);
    QModelIndex index1 = model->index(0, 0);
    EXPECT_EQ(model->data(index1, PreviewRole).toString().toStdString(), "1234567890 1234567890...");
    QString contentNoSpace = "12345678901234567890123456789012345";
    model->AddData(false, false, false, "Theme", "Name", "To", contentNoSpace, "", true);
    QModelIndex index2 = model->index(0, 0);
    EXPECT_EQ(model->data(index2, PreviewRole).toString().toStdString(), "123456789012345678901234567890...");
}

TEST_F(EmailListModelTest, SetStarredTest) {
    int bad_row = -1;
    EXPECT_FALSE(model->SetStarred(bad_row, false));
    EmailData item_false;
    item_false.id = 67;
    model->AddData(item_false);
    EXPECT_FALSE(model->SetStarred(0, false));
    EmailData item_true;
    item_true.id = -1;
    item_true.is_starred = false;
    model->AddData(item_true);
    QModelIndex index = model->index(0, 0);
    EXPECT_TRUE(model->SetStarred(index.row(), true));
    EXPECT_TRUE(model->data(index, StarredRole).toBool());
}

TEST_F(EmailListModelTest, SetDataTest) {
    EXPECT_FALSE(model->setData(QModelIndex(), "Test", ThemeRole));
    EXPECT_FALSE(model->setData(model->index(99, 0), "Test", ThemeRole));
    EmailData item_db_fail;
    item_db_fail.id = 67;
    model->AddData(item_db_fail);
    QModelIndex index_fail = model->index(0, 0);
    EXPECT_FALSE(model->setData(index_fail, true, StarredRole));
    EmailData item_success;
    item_success.id = -1;
    item_success.theme = "Old Theme";
    item_success.is_starred = false;
    model->AddData(item_success);
    QModelIndex index_success = model->index(0, 0);
    EXPECT_TRUE(model->setData(index_success, true, StarredRole));
    EXPECT_TRUE(model->data(index_success, StarredRole).toBool());
    EXPECT_TRUE(model->setData(index_success, "New Theme", ThemeRole));
    EXPECT_EQ(model->data(index_success, ThemeRole).toString().toStdString(), "New Theme");
    EXPECT_TRUE(model->setData(index_success, "admin@test.com", SendToRole));
    EXPECT_EQ(model->data(index_success, SendToRole).toString().toStdString(), "admin@test.com");
    EXPECT_TRUE(model->setData(index_success, "New Content", ContentRole));
    EXPECT_EQ(model->data(index_success, ContentRole).toString().toStdString(), "New Content");
    EXPECT_FALSE(model->setData(index_success, true, InboxRole));
}
