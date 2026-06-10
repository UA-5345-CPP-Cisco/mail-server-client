#pragma once

#include <functional>
#include <future>

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
    std::future<void> Enqueue(std::function<void()> task) override;
};

}
