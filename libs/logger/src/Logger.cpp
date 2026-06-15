#include "logger/Logger.h"

#include <chrono>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <thread>

namespace Logging {

namespace {

const char* ToString(LogLevel level)
{
	switch (level) {
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

}

Logger::Logger(LogLevel minimumLevel, bool flushAfterWrite)
	: minimumLevel_(minimumLevel),
	  flushAfterWrite_(flushAfterWrite)
{
}

Logger::Logger(
	const std::string& filePath,
	LogLevel minimumLevel,
	bool flushAfterWrite)
	: Logger(minimumLevel, flushAfterWrite)
{
	if (!Open(filePath)) {
		throw std::runtime_error("Unable to open log file: " + filePath);
	}
}

bool Logger::Open(const std::string& filePath)
{
	std::ofstream file(filePath, std::ios::out | std::ios::app);
	if (!file.is_open()) {
		return false;
	}

	std::lock_guard<std::mutex> lock(mutex_);
	file_ = std::move(file);
	return true;
}

void Logger::Log(LogLevel level, std::string_view message)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (level < minimumLevel_) {
		return;
	}

	const auto now = std::chrono::system_clock::now().time_since_epoch();
	const auto timestamp =
		std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
	std::ostream& output = file_.is_open() ? file_ : std::clog;

	output << '[' << timestamp << "] [" << std::this_thread::get_id()
		   << "] [" << ToString(level) << "] " << message << '\n';

	if (flushAfterWrite_) {
		output.flush();
	}
}

}
