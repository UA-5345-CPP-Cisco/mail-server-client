#include "thread_pool/ThreadPool.h"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

namespace {

TEST(ThreadPoolTest, ReturnsTaskResult)
{
	Concurrency::ThreadPool threadPool(2);

	std::future<int> result = threadPool.Submit([](int left, int right) {
		return left + right;
	}, 20, 22);

	EXPECT_EQ(result.get(), 42);
}

TEST(ThreadPoolTest, PropagatesTaskExceptions)
{
	Concurrency::ThreadPool threadPool(1);

	std::future<void> result = threadPool.Enqueue([] {
		throw std::runtime_error("task failed");
	});

	EXPECT_THROW(result.get(), std::runtime_error);
}

TEST(ThreadPoolTest, UsesOneWorkerWhenWorkerCountIsZero)
{
	Concurrency::ThreadPool threadPool(0);

	std::future<std::string> result = threadPool.Submit([] {
		return std::string("completed");
	});

	EXPECT_EQ(result.get(), "completed");
}

}
