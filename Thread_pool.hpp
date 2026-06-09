#pragma once

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include<type_traits>
#include<utility>
#include <stdexcept>

class ThreadPool 
{
private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> queue;
	std::mutex queue_mutex;
	std::condition_variable cv;
	bool stop_pool;

	void WorkerLoop();
public:
	ThreadPool()=default;

	~ThreadPool();

	ThreadPool(const ThreadPool& other) = delete;
	ThreadPool operator=(const ThreadPool& other) = delete;

	void ShutDown();

	void Initialize(size_t threads_num);

	template<class F,class... Args> 
	auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>;
};

template<class F, class... Args>
auto ThreadPool::Enqueue(F&& f, Args&&... args)->std::future<typename std::invoke_result<F, Args...>::type>
{
	using return_type = typename std::invoke_result<F,Args...>::type;
	auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f),std::forward<Args>(args)...));
	std::future<return_type> future_res = task_ptr->get_future();
	{
		std::lock_guard<std::mutex> lock_g(this->queue_mutex);
		this->queue.emplace([task_ptr] {(*task_ptr)();});
	}
	this->cv.notify_all();
	return future_res;
}