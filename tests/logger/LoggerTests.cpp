#include <cstdio>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "logger/Logger.h"

namespace {

class LoggerTest : public testing::Test
{
  protected:
  void TearDown() override
  {
    std::remove(filePath_.c_str());
  }

  std::string ReadLog() const
  {
    std::ifstream file(filePath_);
    std::ostringstream contents;
    contents << file.rdbuf();
    return contents.str();
  }

  const std::string filePath_{"logger-test.log"};
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

TEST_F(LoggerTest, WritesFormattedTimestampWithoutProcessId)
{
  Logging::Logger logger(filePath_, Logging::LogLevel::Trace, true);

  logger.Log(Logging::LogLevel::Info, "formatted");

  const std::string contents = ReadLog();
  const std::regex linePattern(R"(\[\d{2}:\d{2}:\d{2}:\d{3} \d{2}/\d{2}/\d{2}\] \[INFO\] formatted\n)");
  EXPECT_TRUE(std::regex_match(contents, linePattern)) << contents;
}

} // namespace
