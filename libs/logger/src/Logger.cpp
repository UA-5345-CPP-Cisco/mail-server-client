#include "logger/Logger.h"
//#include "Logger.h"

/*int main()
{
    auto& logger = Logger::GetInstance();
    logger.Initialize("app.log",LogLevels::Trace,true);
    logger.LogProd("main","Application started" );
    return 0;
}*/

std::string Logger::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::tm tm_info;
    localtime_r(&now_c, &tm_info);
    std::ostringstream oss;
    oss << std::put_time(&tm_info, "%d/%m/%Y %H:%M:%S") << ":" << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

bool Logger::Initialize(const std::string& file_path, LogLevels level, bool flush)
{
    std::lock_guard<std::mutex> lock_m(this->logger_mutex);
    if (this->logger_fp.is_open())
    {
        this->logger_fp.close();
    }
    this->logger_fp.open(file_path, std::ios::out | std::ios::app);
    if (!this->logger_fp.is_open())
    {
        return false;
    }
    this->level = level;
    this->flush = flush;
    return true;
}

void Logger::LogProd(const std::string_view& func, const std::string_view& msg) {
    if (this->level < LogLevels::ProdLogs)
    {
        return;
    }
    std::lock_guard<std::mutex> lock(this->logger_mutex);
    this->logger_fp << std::this_thread::get_id() << " - "<< GetCurrentTime()
                    << " [1] - [" << func << "] " << msg << "\n";
    if (this->flush) {
        this->logger_fp.flush();
    }
}


void Logger::LogDebug(const std::string_view& func, const std::string_view& msg) {
    if (this->level < LogLevels::Debug) {
        return;
    }
    std::lock_guard<std::mutex> lock(this->logger_mutex);
    this->logger_fp << std::this_thread::get_id() << " - "<< GetCurrentTime()<< " [2] - [" << func << "] " << msg << "\n";
    if (this->flush)
    {
        this->logger_fp.flush();
    }
}

void Logger::LogTrace(const std::string_view& func, const std::string_view& input, const std::string_view& ret_val) {
    if (this->level < LogLevels::Trace)
    {
        return;
    }
    std::lock_guard<std::mutex> lock(this->logger_mutex);
    this->logger_fp << std::this_thread::get_id() << " - "<< GetCurrentTime()<< " [3] - [" << func << "] Input: " << input << " | Return: " << ret_val << "\n";
    if (this->flush) {
        this->logger_fp.flush();
    }
}