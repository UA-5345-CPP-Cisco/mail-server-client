#include"Thread_pool.hpp"
#include<iostream>

void ThreadPool::ShutDown()
{
	{
		std::lock_guard<std::mutex>lock_m(this->queue_mutex);
		this->stop_pool = true;
	}
	this->cv.notify_all();
	for (auto& i : workers)
	{
		if (i.joinable())
		{
			i.join();
		}
	}
} 

ThreadPool::~ThreadPool()
{
	this->ShutDown();
}

void ThreadPool::Initialize(size_t threads_num)
{ 
	this->stop_pool = false;
	this->workers.reserve(threads_num);
	for (size_t i = 0; i < threads_num; i++)
	{
		this->workers.emplace_back(&ThreadPool::WorkerLoop, this);
	}
}

void ThreadPool::WorkerLoop()
{
	while (true)
	{
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> unique_m(this->queue_mutex);
			cv.wait(unique_m,[this] {return this->stop_pool || !this->queue.empty();});
			if (this->stop_pool && this->queue.empty())
			{
				return;
			}
			task = this->queue.front();
			this->queue.pop();
		}
		task();
	}
}
