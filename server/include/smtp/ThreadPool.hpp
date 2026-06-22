#pragma once

#include <functional>
#include <future>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace smtp {

// Minimal execution interface used by SmtpServer/SmtpSession scheduling.
// The actual project thread pool already exposes Enqueue(func, ...); an adapter
// can wrap that concrete implementation behind this interface if needed.
class IThreadPool {
public:
    virtual ~IThreadPool() = default;

    virtual std::future<void> Enqueue(std::function<void()> task) = 0;
};

// Adapter-shaped skeleton for the project's concrete/custom thread pool.
// The final version should forward to the already implemented Enqueue(func, ...)
// and return the std::future produced by that implementation.
class ThreadPool : public IThreadPool {
public:
    explicit ThreadPool(std::size_t workerCount = std::thread::hardware_concurrency());
    ~ThreadPool() override;

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    std::future<void> Enqueue(std::function<void()> task) override;

private:
    void Stop();

    std::vector<std::thread> workers_;
    std::queue<std::packaged_task<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool stopping_{false};
};

}
