#include <gtest/gtest.h>
#include<memory>
#include <QStandardItemModel>

#include "headers/search/MessageSearchModel.h"

class MessageSearchModelTests : public ::testing::Test {
    protected:
    std::unique_ptr<ISXMail::MessageSearchModel> model;

    void SetUp() override {
        this->model = std::make_unique<ISXMail::MessageSearchModel>();
    }

    void TearDown() override {
        this->model.reset();
    }
};

//WhenregisterUser_RegistrationHandlerDoesReturnTrue

TEST_F(MessageSearchModelTests,WhenMessageSearchModel_SetSearched) {
    const QString text="Same text";
    this->model->SetSearchedText(text);
    EXPECT_EQ(text, this->model->SearchedText());
}

