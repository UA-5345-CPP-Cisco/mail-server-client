#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include "thread_pool/ThreadPool.h"

namespace {

constexpr auto TestTimeout = std::chrono::seconds(1);

TEST(ThreadPoolTest, ReturnsTaskResult)
{
  Concurrency::ThreadPool threadPool(2);

  auto result = threadPool.Submit([](int left, int right) { return left + right; }, 20, 22);

  EXPECT_EQ(result.get(), 42);
}

TEST(ThreadPoolTest, PropagatesTaskExceptions)
{
  Concurrency::ThreadPool threadPool(1);

  std::future<void> result = threadPool.Enqueue([] { throw std::runtime_error("task failed"); });

  EXPECT_THROW(result.get(), std::runtime_error);
}

TEST(ThreadPoolTest, UsesOneWorkerWhenWorkerCountIsZero)
{
  Concurrency::ThreadPool threadPool(0);

  auto result = threadPool.Submit([] { return std::string("completed"); });

  EXPECT_EQ(result.get(), "completed");
}

TEST(ThreadPoolTest, ExecutesAllQueuedTasks)
{
  Concurrency::ThreadPool threadPool(4);
  std::atomic<int> completedTasks{0};
  std::vector<std::future<void>> results;

  for (int taskIndex = 0; taskIndex < 100; ++taskIndex)
  {
    results.push_back(threadPool.Enqueue([&completedTasks] { ++completedTasks; }));
  }

  for (auto& result : results)
  {
    ASSERT_EQ(result.wait_for(TestTimeout), std::future_status::ready);
    result.get();
  }

  EXPECT_EQ(completedTasks.load(), 100);
}

TEST(ThreadPoolTest, ContinuesExecutingTasksAfterException)
{
  Concurrency::ThreadPool threadPool(2);

  auto failedResult = threadPool.Enqueue([] { throw std::runtime_error("task failed"); });
  auto successfulResult = threadPool.Submit([] { return 7; });

  EXPECT_THROW(failedResult.get(), std::runtime_error);
  EXPECT_EQ(successfulResult.get(), 7);
}

TEST(ThreadPoolTest, SingleWorkerExecutesTasksInSubmissionOrder)
{
  Concurrency::ThreadPool threadPool(1);
  std::vector<int> executionOrder;
  std::vector<std::future<void>> results;

  for (int taskIndex = 0; taskIndex < 5; ++taskIndex)
  {
    results.push_back(threadPool.Enqueue([&executionOrder, taskIndex] {
      executionOrder.push_back(taskIndex);
    }));
  }

  for (auto& result : results)
  {
    ASSERT_EQ(result.wait_for(TestTimeout), std::future_status::ready);
    result.get();
  }

  EXPECT_EQ(executionOrder, std::vector<int>({0, 1, 2, 3, 4}));
}

TEST(ThreadPoolTest, DestructorWaitsForQueuedTasksToFinish)
{
  std::atomic<bool> taskFinished{false};
  std::promise<void> taskStarted;
  std::future<void> taskStartedResult = taskStarted.get_future();

  {
    Concurrency::ThreadPool threadPool(1);
    threadPool.Enqueue([&taskFinished, &taskStarted] {
      taskStarted.set_value();
      std::this_thread::sleep_for(std::chrono::milliseconds(25));
      taskFinished = true;
    });

    ASSERT_EQ(taskStartedResult.wait_for(TestTimeout), std::future_status::ready);
  }

  EXPECT_TRUE(taskFinished.load());
}

TEST(ThreadPoolTest, RunsTasksConcurrentlyWhenMultipleWorkersAreAvailable)
{
  Concurrency::ThreadPool threadPool(2);
  std::mutex mutex;
  std::condition_variable condition;
  int startedTasks = 0;
  bool canFinish = false;

  auto blockingTask = [&] {
    std::unique_lock<std::mutex> lock(mutex);
    ++startedTasks;
    condition.notify_all();
    condition.wait(lock, [&canFinish] { return canFinish; });
  };

  auto firstResult = threadPool.Enqueue(blockingTask);
  auto secondResult = threadPool.Enqueue(blockingTask);

  {
    std::unique_lock<std::mutex> lock(mutex);
    EXPECT_TRUE(condition.wait_for(lock, TestTimeout, [&startedTasks] {
      return startedTasks == 2;
    }));
    canFinish = true;
  }

  condition.notify_all();
  ASSERT_EQ(firstResult.wait_for(TestTimeout), std::future_status::ready);
  ASSERT_EQ(secondResult.wait_for(TestTimeout), std::future_status::ready);
  firstResult.get();
  secondResult.get();
}

} // namespace