#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <unistd.h>
#include "Server.hpp"

class ImapTest : public ::testing::Test
{
protected:
    int fds[2];
    Server test_server;

    ImapTest() : test_server(4) {}

    void SetUp() override
    {
        ASSERT_EQ(socketpair(AF_UNIX, SOCK_STREAM, 0, fds), 0);
    }

    void TearDown() override
    {
        close(fds[0]);
        close(fds[1]);
    }

    void TestSetServerSocket()
    {
        test_server.SetServerSocket(0);
    }

    void TestSetEpoll()
    {
        test_server.SetEpoll();
    }

    std::unordered_map<int, std::shared_ptr<Client>> &TestTimeOuts()
    {
        test_server.clinets.emplace(5, std::make_shared<Client>(5));
        auto it=test_server.clinets.find(5);
        it->second->last_activity=time(NULL);
        test_server.clinets.emplace(6,std::make_shared<Client>(6));
        auto it1=test_server.clinets.find(6);
        it1->second->last_activity=time(NULL)- TIMEOUT - 10;
        test_server.CheakTimeOut();
        return test_server.clinets;
    }

    std::string TestProcessRequest(const char *raw_json)
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        test_server.ProcessRequest(fds[0], raw_json);
        recv(fds[1], buffer, sizeof(buffer) - 1, 0);
        return std::string(buffer);
    }
};

TEST_F(ImapTest, StartingTest)
{
    EXPECT_NO_THROW(TestSetServerSocket());
    EXPECT_NO_THROW(TestSetEpoll());
}

TEST_F(ImapTest, TestTimeOuts)
{
    EXPECT_EQ(TestTimeOuts().size(), 1);
}

TEST_F(ImapTest, SendingMsgTests)
{
    const char *raw_json1 = "{\"action\": \"LOGIN\", \"email\": \"test@mail.com\", \"password\": \"1234\"}";
    std::string res1 = "{\"status\": \"OK\", \"message\": \"Logged in successfully\"}\n";
    EXPECT_EQ(TestProcessRequest(raw_json1), res1);
    const char *raw_json2 = "{\"action\": \"BLABLA\"}";
    std::string res2 = "{\"status\": \"ERROR\", \"message\": \"Unknown action\"}\n";
    EXPECT_EQ(TestProcessRequest(raw_json2), res2);
}

TEST_F(ImapTest, LoginMissingFields)
{
    const char *raw_json = "{\"action\": \"LOGIN\"}";
    std::string result = TestProcessRequest(raw_json);
    EXPECT_FALSE(result.empty());
    EXPECT_NE(result.find("status"), std::string::npos);
}

TEST_F(ImapTest, InvalidJson)
{
    const char *raw_json = "this is not json at all";
    std::string result = TestProcessRequest(raw_json);
    EXPECT_EQ(result, "{\"status\": \"ERROR\", \"message\": \"Invalid JSON format\"}\n");
}

TEST_F(ImapTest, MissingActionField)
{
    const char *raw_json = "{\"email\": \"test@mail.com\"}";
    std::string result = TestProcessRequest(raw_json);
    EXPECT_EQ(result, "{\"status\": \"ERROR\", \"message\": \"Missing 'action' field\"}\n");
}

TEST_F(ImapTest, NotJsonObject)
{
    const char *raw_json = "[1, 2, 3]";
    std::string result = TestProcessRequest(raw_json);
    EXPECT_EQ(result, "{\"status\": \"ERROR\", \"message\": \"Invalid JSON structure\"}\n");
}

TEST_F(ImapTest, ListEmailsAction)
{
    const char *raw_json = "{\"action\": \"LIST_EMAILS\"}";
    std::string result = TestProcessRequest(raw_json);
    EXPECT_NE(result.find("OK"), std::string::npos);
    EXPECT_NE(result.find("emails"), std::string::npos);
}

TEST_F(ImapTest, ReadEmailAction)
{
    const char *raw_json = "{\"action\": \"READ_EMAIL\", \"email_id\": 1}";
    std::string result = TestProcessRequest(raw_json);
    EXPECT_NE(result.find("OK"), std::string::npos);
    EXPECT_NE(result.find("body"), std::string::npos);
}

TEST_F(ImapTest, ReadEmailMissingId)
{
    const char *raw_json = "{\"action\": \"READ_EMAIL\"}";
    std::string result = TestProcessRequest(raw_json);
    EXPECT_FALSE(result.empty());
}

TEST_F(ImapTest, QuitAction)
{
    const char *raw_json = "{\"action\": \"QUIT\"}";
    std::string result = TestProcessRequest(raw_json);
    EXPECT_EQ(result, "{\"status\": \"OK\", \"message\": \"Goodbye\"}\n");
}
