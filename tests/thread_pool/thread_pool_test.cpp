#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "thread_pool/ThreadPool.h"

class ThreadPoolTest : public ::testing::Test
{
protected:
    ThreadPool pool;

    void SetUp() override
    {
        pool.Initialize(4);
    }

    void TearDown() override
    {
        pool.ShutDown();
    }

    template <class F, class... Args>
    auto TestEnqueue(F &&f, Args... argc)
    {
        return pool.Enqueue(std::forward<F>(f), std::forward<Args>(argc)...);
    }
};

TEST_F(ThreadPoolTest, SimpleTaskExecution)
{
    auto future = TestEnqueue([](int a, int b)
                              { return a + b; }, 5, 7);
    EXPECT_EQ(future.get(), 12);
}

TEST_F(ThreadPoolTest, MultipleTasks)
{
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 20; ++i)
    {
        futures.push_back(TestEnqueue([](int x)
                                      { return x * 2; }, i));
    }

    for (int i = 0; i < 20; ++i)
    {
        EXPECT_EQ(futures[i].get(), i * 2);
    }
}