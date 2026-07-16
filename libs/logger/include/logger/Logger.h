#pragma once

#include <fstream>
#include <mutex>
#include <string>
#include <string_view>

namespace Logging {

enum class LogLevel
{
  Trace,
  Debug,
  Info,
  Warning,
  Error
};

class ILogger
{
  public:
  virtual ~ILogger() = default;

  virtual void Log(LogLevel level, std::string_view message) = 0;
};

class Logger final : public ILogger
{
  public:
  static Logger& Instance()
  {
    static Logger instance(LogLevel::Trace, false);
    return instance;
  }
  explicit Logger(LogLevel minimumLevel = LogLevel::Info, bool flushAfterWrite = false);

  Logger(const std::string& filePath,
         LogLevel minimumLevel = LogLevel::Info,
         bool flushAfterWrite = false);

  bool Open(const std::string& filePath);
  void Log(LogLevel level, std::string_view message) override;

  private:
  std::mutex mutex_;
  std::ofstream file_;
  LogLevel minimumLevel_;
  bool flushAfterWrite_;
};

} // namespace Logging
