#include "smtp/ThreadPool.hpp"

#include <gtest/gtest.h>

#include <atomic>
#include <stdexcept>

namespace smtp::test {

TEST(ThreadPoolTests, RunsQueuedTasks)
{
    ThreadPool pool(2);
    std::atomic_int counter{0};

    std::future<void> first = pool.Enqueue([&counter] {
        ++counter;
    });
    std::future<void> second = pool.Enqueue([&counter] {
        ++counter;
    });

    first.get();
    second.get();

    EXPECT_EQ(counter.load(), 2);
}

TEST(ThreadPoolTests, ZeroWorkersFallsBackToOneWorker)
{
    ThreadPool pool(0);
    std::atomic_bool ran{false};

    std::future<void> task = pool.Enqueue([&ran] {
        ran.store(true);
    });

    task.get();

    EXPECT_TRUE(ran.load());
}

TEST(ThreadPoolTests, TaskExceptionIsReportedThroughFuture)
{
    ThreadPool pool(1);

    std::future<void> task = pool.Enqueue([] {
        throw std::runtime_error("boom");
    });

    EXPECT_THROW(task.get(), std::runtime_error);
}

}
