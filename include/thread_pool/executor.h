#pragma once

#include <memory>
#include <thread_safe_queue.h>
#include <thread>
#include <functional>

#include "task.h"

// class IThreadPool {
// };

inline void worker_thread(const std::shared_ptr<ThreadSafeQueue<Task> > &queue) {
}

class Executor {
public:
    explicit Executor(const int thread_num = 4) : queue_(std::make_shared<ThreadSafeQueue<Task> >()) {
        for (int i = 0; i < thread_num; i++)
            workers.emplace_back(worker_thread, queue_);
    };

    ~Executor() = default; // TODO: graceful shutdown? idk if here

    template<typename T>
    std::shared_ptr<Task<T> > compute(std::function<T> f);

private:
    std::vector<std::thread> workers;
    std::shared_ptr<ThreadSafeQueue<Task> > queue_;
};
