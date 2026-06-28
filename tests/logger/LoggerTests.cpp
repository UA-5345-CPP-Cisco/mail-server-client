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
  auto& logger = Logger::GetInstance();
  ASSERT_TRUE(logger.Initialize(filePath_, LogLevels::ProdLogs, true));

  logger.LogDebug("TestFunc", "hidden");
  logger.LogProd("TestFunc", "visible");

  const std::string contents = ReadLog();
  EXPECT_EQ(contents.find("hidden"), std::string::npos);
  EXPECT_NE(contents.find("visible"), std::string::npos);
}

TEST_F(LoggerTest, AppendsToExistingLogFile)
{
  auto& logger = Logger::GetInstance();
  ASSERT_TRUE(logger.Initialize(filePath_, LogLevels::Trace, true));

  logger.LogProd("TestFunc", "first");
  logger.LogProd("TestFunc", "second");

  const std::string contents = ReadLog();
  EXPECT_NE(contents.find("first"), std::string::npos);
  EXPECT_NE(contents.find("second"), std::string::npos);
}

} // namespace
