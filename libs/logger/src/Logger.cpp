#include "logger/Logger.h"

#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace Logging {

namespace {

const char* ToString(LogLevel level)
{
  switch (level)
  {
  case LogLevel::Trace:
    return "TRACE";
  case LogLevel::Debug:
    return "DEBUG";
  case LogLevel::Info:
    return "INFO";
  case LogLevel::Warning:
    return "WARN";
  case LogLevel::Error:
    return "ERROR";
  }

  return "UNKNOWN";
}

std::string Timestamp()
{
  const boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
  const boost::gregorian::date date = now.date();
  const boost::posix_time::time_duration time = now.time_of_day();

  const long fractional_seconds = time.fractional_seconds();
  const int fractional_digits = boost::posix_time::time_duration::num_fractional_digits();
  int milliseconds = static_cast<int>(fractional_seconds);
  for (int digits = fractional_digits; digits > 3; --digits)
  {
    milliseconds /= 10;
  }

  std::ostringstream output;
  output << std::setfill('0') << std::setw(2) << time.hours() << ":" << std::setw(2) << time.minutes()
         << ":" << std::setw(2) << time.seconds() << ":" << std::setw(3) << milliseconds << ' ' << std::setw(2)
         << date.year() % 100 << '/' << std::setw(2) << static_cast<int>(date.month()) << '/' << std::setw(2)
         << static_cast<int>(date.day());

  return output.str();
}

} // namespace

Logger::Logger(LogLevel minimumLevel, bool flushAfterWrite) :
  minimumLevel_(minimumLevel),
  flushAfterWrite_(flushAfterWrite)
{
}

Logger::Logger(const std::string& filePath, LogLevel minimumLevel, bool flushAfterWrite) :
  Logger(minimumLevel, flushAfterWrite)
{
  if (!Open(filePath))
  {
    throw std::runtime_error("Unable to open log file: " + filePath);
  }
}

bool Logger::Open(const std::string& filePath)
{
  std::ofstream file(filePath, std::ios::out | std::ios::app);
  if (!file.is_open())
  {
    return false;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  file_ = std::move(file);
  return true;
}

void Logger::Log(LogLevel level, std::string_view message)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (level < minimumLevel_)
  {
    return;
  }

  std::ostream& output = file_.is_open() ? file_ : std::clog;

  output << '[' << Timestamp() << "] [" << ToString(level) << "] " << message << '\n';

  if (flushAfterWrite_)
  {
    output.flush();
  }
}

} // namespace Logging
