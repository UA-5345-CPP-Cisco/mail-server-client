#include <cstdio>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "logger/Logger.h"

namespace {

class LoggerTest : public testing::Test
{
  protected:
  void SetUp() override
  {
    const testing::TestInfo* testInfo = testing::UnitTest::GetInstance()->current_test_info();
    filePath_ = std::string(testInfo->test_suite_name()) + "-" + testInfo->name() + ".log";
    secondFilePath_ = filePath_ + ".second";
    std::remove(filePath_.c_str());
    std::remove(secondFilePath_.c_str());
  }

  void TearDown() override
  {
    std::remove(filePath_.c_str());
    std::remove(secondFilePath_.c_str());
  }

  std::string ReadLog() const
  {
    std::ifstream file(filePath_);
    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
  }

  std::string ReadSecondLog() const
  {
    std::ifstream file(secondFilePath_);
    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
  }

  static int CountLines(const std::string& contents)
  {
    std::istringstream stream(contents);
    int lineCount = 0;
    std::string line;
    while (std::getline(stream, line))
    {
      ++lineCount;
    }

    return lineCount;
  }

  std::string filePath_;
  std::string secondFilePath_;
};

TEST_F(LoggerTest, WritesMessagesAtOrAboveMinimumLevel)
{
  Logging::Logger logger(filePath_, Logging::LogLevel::Info, true);

  logger.Log(Logging::LogLevel::Debug, "hidden");
  logger.Log(Logging::LogLevel::Info, "visible");

  const std::string contents = ReadLog();
  EXPECT_EQ(contents.find("hidden"), std::string::npos);
  EXPECT_NE(contents.find("visible"), std::string::npos);
}

TEST_F(LoggerTest, WritesAllLogLevelLabels)
{
  Logging::Logger logger(filePath_, Logging::LogLevel::Trace, true);

  logger.Log(Logging::LogLevel::Trace, "trace message");
  logger.Log(Logging::LogLevel::Debug, "debug message");
  logger.Log(Logging::LogLevel::Info, "info message");
  logger.Log(Logging::LogLevel::Warning, "warning message");
  logger.Log(Logging::LogLevel::Error, "error message");

  const std::string contents = ReadLog();
  EXPECT_NE(contents.find("[TRACE] trace message"), std::string::npos);
  EXPECT_NE(contents.find("[DEBUG] debug message"), std::string::npos);
  EXPECT_NE(contents.find("[INFO] info message"), std::string::npos);
  EXPECT_NE(contents.find("[WARN] warning message"), std::string::npos);
  EXPECT_NE(contents.find("[ERROR] error message"), std::string::npos);
}

TEST_F(LoggerTest, WritesExpectedLogLineFormat)
{
  Logging::Logger logger(filePath_, Logging::LogLevel::Info, true);

  logger.Log(Logging::LogLevel::Info, "formatted message");

  const std::string contents = ReadLog();
  const std::regex expectedFormat(R"(\[[0-9]+\] \[[^\]]+\] \[INFO\] formatted message)");
  EXPECT_TRUE(std::regex_search(contents, expectedFormat));
}

TEST_F(LoggerTest, AppendsToExistingLogFile)
{
  {
    Logging::Logger logger(filePath_, Logging::LogLevel::Trace, true);
    logger.Log(Logging::LogLevel::Error, "first");
  }
  {
    Logging::Logger logger(filePath_, Logging::LogLevel::Trace, true);
    logger.Log(Logging::LogLevel::Warning, "second");
  }

  const std::string contents = ReadLog();
  EXPECT_NE(contents.find("first"), std::string::npos);
  EXPECT_NE(contents.find("second"), std::string::npos);
}

TEST_F(LoggerTest, OpenReturnsTrueAndWritesToOpenedFile)
{
  Logging::Logger logger(Logging::LogLevel::Trace, true);

  EXPECT_TRUE(logger.Open(filePath_));
  logger.Log(Logging::LogLevel::Error, "opened later");

  const std::string contents = ReadLog();
  EXPECT_NE(contents.find("[ERROR] opened later"), std::string::npos);
}

TEST_F(LoggerTest, OpenReturnsFalseWhenFileCannotBeOpened)
{
  Logging::Logger logger(Logging::LogLevel::Trace, true);

  EXPECT_FALSE(logger.Open("missing-logger-directory/logger-test.log"));
}

TEST_F(LoggerTest, ConstructorThrowsWhenFileCannotBeOpened)
{
  EXPECT_THROW(
    Logging::Logger("missing-logger-directory/logger-test.log", Logging::LogLevel::Info, true),
    std::runtime_error);
}

TEST_F(LoggerTest, OpenSwitchesOutputFile)
{
  Logging::Logger logger(filePath_, Logging::LogLevel::Trace, true);
  logger.Log(Logging::LogLevel::Info, "first file");

  ASSERT_TRUE(logger.Open(secondFilePath_));
  logger.Log(Logging::LogLevel::Info, "second file");

  const std::string firstContents = ReadLog();
  const std::string secondContents = ReadSecondLog();
  EXPECT_NE(firstContents.find("first file"), std::string::npos);
  EXPECT_EQ(firstContents.find("second file"), std::string::npos);
  EXPECT_NE(secondContents.find("second file"), std::string::npos);
}

TEST_F(LoggerTest, WritesCompleteLinesFromMultipleThreads)
{
  constexpr int ThreadCount = 8;
  constexpr int MessagesPerThread = 25;

  {
    Logging::Logger logger(filePath_, Logging::LogLevel::Trace, true);
    std::vector<std::thread> threads;
    threads.reserve(ThreadCount);

    for (int threadIndex = 0; threadIndex < ThreadCount; ++threadIndex)
    {
      threads.emplace_back(
        [&logger, threadIndex]
        {
          for (int messageIndex = 0; messageIndex < MessagesPerThread; ++messageIndex)
          {
            logger.Log(Logging::LogLevel::Info,
                       "thread " + std::to_string(threadIndex) + " message " +
                         std::to_string(messageIndex));
          }
        });
    }

    for (std::thread& thread : threads)
    {
      thread.join();
    }
  }

  const std::string contents = ReadLog();
  EXPECT_EQ(CountLines(contents), ThreadCount * MessagesPerThread);
  EXPECT_NE(contents.find("thread 0 message 0"), std::string::npos);
  EXPECT_NE(contents.find("thread 7 message 24"), std::string::npos);
}

} // namespace