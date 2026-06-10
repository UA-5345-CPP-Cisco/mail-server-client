#include "smtp/ThreadPool.hpp"

#include <stdexcept>
#include <utility>

namespace smtp {

ThreadPool::ThreadPool(std::size_t workerCount)
{
    if (workerCount == 0) {
        workerCount = 1;
    }

    workers_.reserve(workerCount);
    for (std::size_t index = 0; index < workerCount; ++index) {
        workers_.emplace_back([this] {
            for (;;) {
                std::packaged_task<void()> task;

                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    condition_.wait(lock, [this] {
                        return stopping_ || !tasks_.empty();
                    });

                    if (stopping_ && tasks_.empty()) {
                        return;
                    }

                    task = std::move(tasks_.front());
                    tasks_.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    Stop();
}

std::future<void> ThreadPool::Enqueue(std::function<void()> task)
{
    std::packaged_task<void()> packagedTask(std::move(task));
    std::future<void> future = packagedTask.get_future();

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stopping_) {
            throw std::runtime_error("ThreadPool is stopping");
        }

        tasks_.push(std::move(packagedTask));
    }

    condition_.notify_one();
    return future;
}

void ThreadPool::Stop()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);
        stopping_ = true;
    }

    condition_.notify_all();

    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

}
