#include <gtest/gtest.h>

#include <QStandardItemModel>
#include <QStandardItem>


#include "headers/mail/EmailFilterProxy.h"
#include "headers/mail/EmailListModel.h"

class EmailFilterProxyTest : public ::testing::Test {
protected:
    QStandardItemModel sourceModel;

    void SetUp() override {
        sourceModel.insertRows(0, 5);
        sourceModel.insertColumns(0, 1);
        auto setRowData = [this](int row, bool inbox, bool sent, bool draft, bool starred) {
            QModelIndex idx = sourceModel.index(row, 0);
            sourceModel.setData(idx, inbox, ISXMail::InboxRole);
            sourceModel.setData(idx, sent, ISXMail::SentRole);
            sourceModel.setData(idx, draft, ISXMail::DraftRole);
            sourceModel.setData(idx, starred, ISXMail::StarredRole);
        };
        setRowData(0, true, false, false, false);
        setRowData(1, false, true, false, false);
        setRowData(2, false, false, true, false);
        setRowData(3, false, true, false, true);
        setRowData(4, false, false, false, false);
    }
};

TEST_F(EmailFilterProxyTest, InboxShowsCorrectEmails) {
    ISXMail::EmailFilterProxy proxy(ISXMail::EmailFilterProxy::Inbox);
    proxy.setSourceModel(&sourceModel);
    EXPECT_EQ(proxy.rowCount(), 2);
}


TEST_F(EmailFilterProxyTest, SentShowsCorrectEmails) {
    ISXMail::EmailFilterProxy proxy(ISXMail::EmailFilterProxy::Sent);
    proxy.setSourceModel(&sourceModel);
    EXPECT_EQ(proxy.rowCount(), 2);
}

TEST_F(EmailFilterProxyTest, StarredShowsOnlyStarredEmails) {
    ISXMail::EmailFilterProxy proxy(ISXMail::EmailFilterProxy::Starred);
    proxy.setSourceModel(&sourceModel);
    EXPECT_EQ(proxy.rowCount(), 1);
}

TEST_F(EmailFilterProxyTest, DraftShowsOnlyDrafts) {
    ISXMail::EmailFilterProxy proxy(ISXMail::EmailFilterProxy::Draft);
    proxy.setSourceModel(&sourceModel);
    EXPECT_EQ(proxy.rowCount(), 1);
}
