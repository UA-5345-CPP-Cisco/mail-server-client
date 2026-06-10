# ThreadPool Component

A thread-safe Thread Pool designed for parallel task execution. This is a core foundational module for our upcoming SMTP server. Instead of creating a new thread for every incoming request, it manages a pool of worker threads that continuously process tasks from a thread-safe queue.

## Key Features
* **Explicit Initialization:** Following the project guidelines ("no heavy constructors"), the pool is initialized via the `Initialize(size_t threads_num)` method.
* **Universal Task Queue:** The `Enqueue` method is heavily templated to accept any callable object (functions, lambdas, `std::bind`) with any number of arguments.
* **Future-Based Results:** Returns a `std::future`, allowing you to safely retrieve the result of an asynchronous task once it completes.
* **Thread-Safe & Non-Copyable:** Fully synchronized using `std::mutex` and `std::condition_variable`. Copying is explicitly disabled to prevent resource management conflicts.

## Quick Start

Here is a basic example of how to integrate and use the `ThreadPool` in your code:

```cpp
#include "Thread_pool.hpp"
#include <iostream>

int main() {
    // 1. Create the ThreadPool instance
    ThreadPool pool;

    // 2. Initialize the pool with the desired number of threads (e.g., 4)
    pool.Initialize(4);

    // 3. Enqueue a task (e.g., a simple lambda function)
    // Enqueue returns a std::future to get the result later
    auto future_result = pool.Enqueue([](int a, int b) {
        return a + b;
    }, 10, 5);

    // 4. Retrieve the result (this will block the main thread until the task is done)
    std::cout << "Task result: " << future_result.get() << std::endl;

    // 5. Gracefully shut down the pool (optional, the destructor handles this automatically)
    pool.ShutDown();

    return 0;
}
