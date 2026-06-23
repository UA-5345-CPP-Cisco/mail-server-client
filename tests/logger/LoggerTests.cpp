#include <cstdio>
#include <fstream>
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
  {
    Logging::Logger logger(filePath_, Logging::LogLevel::Info, true);
    logger.Log(Logging::LogLevel::Debug, "hidden");
    logger.Log(Logging::LogLevel::Info, "visible");
  }

  const std::string contents = ReadLog();
  EXPECT_EQ(contents.find("hidden"), std::string::npos);
  EXPECT_NE(contents.find("[INFO] visible"), std::string::npos);
}

TEST_F(LoggerTest, AppendsToExistingLogFile)
{
  {
    Logging::Logger logger(filePath_);
    logger.Log(Logging::LogLevel::Error, "first");
  }
  {
    Logging::Logger logger(filePath_);
    logger.Log(Logging::LogLevel::Warning, "second");
  }

  const std::string contents = ReadLog();
  EXPECT_NE(contents.find("first"), std::string::npos);
  EXPECT_NE(contents.find("second"), std::string::npos);
}

} // namespace
