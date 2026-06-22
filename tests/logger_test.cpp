#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Logger.hpp"

static std::string ReadFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

TEST(LoggerTests, InitializeValidFile)
{
    auto& logger = Logger::GetInstance();
    EXPECT_TRUE(
        logger.Initialize(
            "test.log",
            LogLevels::Trace,
            true
        )
    );
}

TEST(LoggerTests, WriteProdLog)
{
    const std::string file = "prod.log";
    auto& logger = Logger::GetInstance();
    ASSERT_TRUE(
        logger.Initialize(
            file,
            LogLevels::ProdLogs,
            true
        )
    );
    logger.LogProd("TestFunc", "Hello");
    std::string content = ReadFile(file);
    EXPECT_NE(content.find("TestFunc"), std::string::npos);
    EXPECT_NE(content.find("Hello"), std::string::npos);
    EXPECT_NE(content.find("[1]"), std::string::npos);
}

TEST(LoggerTests, DebugFilteredOut)
{
    const std::string file = "debug_filter.log";
    auto& logger = Logger::GetInstance();
    ASSERT_TRUE(
        logger.Initialize(
            file,
            LogLevels::ProdLogs,
            true
        )
    );
    logger.LogDebug("Func", "DebugMessage");
    std::string content = ReadFile(file);
    EXPECT_EQ(content.find("DebugMessage"), std::string::npos);
}

TEST(LoggerTests, TraceWritten)
{
    const std::string file = "trace.log";
    auto& logger = Logger::GetInstance();
    ASSERT_TRUE(
        logger.Initialize(
            file,
            LogLevels::Trace,
            true
        )
    );
    logger.LogTrace(
        "Add",
        "1,2",
        "3"
    );
    std::string content = ReadFile(file);
    EXPECT_NE(content.find("Input: 1,2"), std::string::npos);
    EXPECT_NE(content.find("Return: 3"), std::string::npos);
    EXPECT_NE(content.find("[3]"), std::string::npos);
}