#include<gtest/gtest.h>
#include <memory>

#include "mail/MessageComposer.h"
#include "database/DatabaseManager.h"

//WhenUseCaseTargetClassDoesX

class MessageComposerTests : public ::testing::Test {
    protected:
    std::unique_ptr<Storage::Database> db;
    std::unique_ptr<Storage::MailMessageRepository> reposit_msg;
    std::unique_ptr<Storage::MailMessageActorRepository> reposit_recp;
    std::unique_ptr<ISXMail::MessageComposer> composer;
    void SetUp() override {
        ISXDatabaseManager::DatabaseManager::EnsureInitialized();
        this->db=std::make_unique<Storage::Database>(ISXDatabaseManager::DatabaseManager::DatabasePath());
        this->db->Execute("DELETE FROM mail_messages;");
        this->reposit_msg=std::make_unique<Storage::MailMessageRepository>(*this->db);
        this->reposit_recp=std::make_unique<Storage::MailMessageActorRepository>(*this->db);
        this->composer=std::make_unique<ISXMail::MessageComposer>();
    }

    void TearDown() override {
        this->reposit_recp.reset();
        this->reposit_msg.reset();
        this->db.reset();
        this->composer.reset();
    }
};

TEST_F(MessageComposerTests,WhenSendMailMessage_MessageComposerDoesReturnFalse) {
    const QString recipient_email_false="   \n   ";
    const QString body_false="   \n   ";
    EXPECT_FALSE(this->composer->SendMailMessage("",
        "",recipient_email_false,"","Some text"));
    EXPECT_FALSE(this->composer->SendMailMessage("",
        "","some tet","",body_false));
}

TEST_F(MessageComposerTests,WhenSaveDraft_MessageComposerDoesReturnFalse) {
    const QString recipient_email="   \n   ";
    const QString subject="   \n   ";
    const QString body="   \n   ";
    EXPECT_FALSE(this->composer->SaveDraft("","",recipient_email,subject,body));
}

TEST_F(MessageComposerTests,WhenSaveDraft_MessageComposerDoesThrow) {
    this->db->Execute("BEGIN EXCLUSIVE;");
    EXPECT_ANY_THROW(this->composer->SaveDraft("",
       "","Some text","Some text","Some text"));
}

TEST_F(MessageComposerTests,WhenSaveDraft_MessageComposerDoesReturnTrue){
    const QString sender_name="Bob";
    const QString sender_email="bob@bob";
    const QString recipient_email="Bob";
    const QString subject="bob";
    const QString body="Some text";
    EXPECT_TRUE(this->composer->SaveDraft(sender_name,
      sender_email,recipient_email,subject,body));
    auto messages=this->reposit_msg->FindAll();
    ASSERT_EQ(1,messages.size());
    auto message=this->reposit_recp->FindByMessageId(messages[0].id);
    EXPECT_EQ(message.size(),2);
    EXPECT_EQ(message[1].actor_email, recipient_email.toStdString());
}
