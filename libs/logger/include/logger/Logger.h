#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string>  
#include<string_view>
#include <fstream>
#include <mutex>   
#include <thread>  
#include<chrono> 
#include<iomanip>
#include <sstream>

enum class LogLevels
{
	NoLogs=0,
	ProdLogs=1,
	Debug=2,
	Trace=3
};

class Logger
{
private:
	std::mutex logger_mutex;
	std::ofstream logger_fp;
	bool flush;
	LogLevels level;

	Logger() = default;
	~Logger() = default;
	Logger(const Logger& other) = delete;
	Logger& operator=(const Logger& other) = delete;
	std::string GetCurrentTime();
public:
	static Logger& GetInstance()
	{
		static Logger logger_obj;
		return logger_obj;
	}

	bool Initialize(const std::string& file_path, LogLevels level, bool flush);

	void LogProd(const std::string_view& func, const std::string_view& msg);

	void LogDebug(const std::string_view& func, const std::string_view& msg);

	void LogTrace(const std::string_view& func, const std::string_view& input, const std::string_view& ret_val);
};