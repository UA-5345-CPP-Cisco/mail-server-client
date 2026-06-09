#pragma once

#include <functional>
#include <future>

namespace smtp {

class IThreadPool {
public:
    virtual ~IThreadPool() = default;

    virtual std::future<void> Enqueue(std::function<void()> task) = 0;
};

}
