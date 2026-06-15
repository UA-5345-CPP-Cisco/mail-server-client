#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace Concurrency {

class IThreadPool {
public:
	virtual ~IThreadPool() = default;

	virtual std::future<void> Enqueue(std::function<void()> task) = 0;
};

class ThreadPool final : public IThreadPool {
public:
	explicit ThreadPool(
		std::size_t workerCount = std::thread::hardware_concurrency());
	~ThreadPool() override;

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	std::future<void> Enqueue(std::function<void()> task) override;

	template<class Function, class... Arguments>
	auto Submit(Function&& function, Arguments&&... arguments)
		-> std::future<std::invoke_result_t<Function, Arguments...>>;

private:
	void AddTask(std::function<void()> task);
	void Stop();
	void WorkerLoop();

	std::vector<std::thread> workers_;
	std::queue<std::function<void()>> tasks_;
	std::mutex mutex_;
	std::condition_variable condition_;
	bool stopping_{false};
};

template<class Function, class... Arguments>
auto ThreadPool::Submit(Function&& function, Arguments&&... arguments)
	-> std::future<std::invoke_result_t<Function, Arguments...>>
{
	using Result = std::invoke_result_t<Function, Arguments...>;

	auto task = std::make_shared<std::packaged_task<Result()>>(
		std::bind(
			std::forward<Function>(function),
			std::forward<Arguments>(arguments)...));
	std::future<Result> result = task->get_future();

	AddTask([task] {
		(*task)();
	});

	return result;
}

}
