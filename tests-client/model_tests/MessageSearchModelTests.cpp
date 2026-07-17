#include <gtest/gtest.h>
#include <memory>
#include <QStandardItemModel>

#include "search/MessageSearchModel.h"

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

TEST_F(MessageSearchModelTests,WhenMessageSearchModel_SetSearched) {
    const QString text="Same text";
    this->model->SetSearchedText(text);
    EXPECT_EQ(text, this->model->SearchedText());
}

TEST_F(MessageSearchModelTests, WhenHighlightAllFoundWords_WithEmptySearch_DoesReturnEscapedOriginalText) {
    const QString text = "Hello world";
    const QString search = "";
    EXPECT_EQ("Hello world", this->model->HighlightAllFoundWords(text, search));
}

TEST_F(MessageSearchModelTests, WhenHighlightAllFoundWords_WithWhitespaceSearch_DoesReturnEscapedOriginalText) {
    const QString text = "Hello world";
    const QString search = "   ";
    EXPECT_EQ("Hello world", this->model->HighlightAllFoundWords(text, search));
}

TEST_F(MessageSearchModelTests, WhenHighlightAllFoundWords_WithNoMatch_DoesReturnEscapedOriginalText) {
    const QString text = "Hello world";
    const QString search = "apple";

    EXPECT_EQ("Hello world", this->model->HighlightAllFoundWords(text, search));
}

TEST_F(MessageSearchModelTests, WhenHighlightAllFoundWords_WithSingleMatch_DoesReturnHighlightedText) {
    const QString text = "Hello world here";
    const QString search = "world";
    const QString expected = "Hello <span style=\"background-color:#fff3a3;color:#101828\">world</span> here";
    EXPECT_EQ(expected, this->model->HighlightAllFoundWords(text, search));
}

TEST_F(MessageSearchModelTests, WhenHighlightAllFoundWords_WithDifferentCase_DoesMatchAndKeepOriginalCase) {
    const QString text = "Hello WORLD";
    const QString search = "world";
    const QString expected = "Hello <span style=\"background-color:#fff3a3;color:#101828\">WORLD</span>";
    EXPECT_EQ(expected, this->model->HighlightAllFoundWords(text, search));
}
TEST_F(MessageSearchModelTests, WhenHighlightAllFoundWords_WithMultipleMatches_DoesHighlightAllMatches) {
    const QString text = "test one test two test";
    const QString search = "test";
    const QString spanOpen = "<span style=\"background-color:#fff3a3;color:#101828\">";
    const QString spanClose = "</span>";
    const QString expected = spanOpen + "test" + spanClose +
                             " one " +
                             spanOpen + "test" + spanClose +
                             " two " +
                             spanOpen + "test" + spanClose;
    EXPECT_EQ(expected, this->model->HighlightAllFoundWords(text, search));
}
