#pragma once

#include <memory>
#include <thread_safe_queue.h>
#include <thread>
#include <functional>

#include "task.h"

// class IThreadPool {
// };


class Executor {
public:
    explicit Executor(const int thread_num = 4) : queue_(std::make_shared<ThreadSafeQueue<ITask> >()) {
        for (int i = 0; i < thread_num; i++)
            workers.emplace_back(worker_thread, queue_);
    };

    ~Executor() = default; // TODO: graceful shutdown? idk if here

    template<typename R, typename Func, typename... Args>
    std::shared_ptr<Task<R> > compute(Func &&f, Args &&... args) {
        std::function<R()> task_function = std::bind(f, args);
        auto task = std::make_shared<Task<R> >(Task<R>(task_function));
        queue_->enqueue(task);
        return task;
    }

private:
    std::vector<std::thread> workers;
    std::shared_ptr<ThreadSafeQueue<ITask> > queue_{}; // TODO queue of shared_ptr?

    [[noreturn]] static void worker_thread(const std::shared_ptr<ThreadSafeQueue<ITask> > &queue) {
        while (true) {
            // TODO change while
            if (queue->is_empty()) continue;
            ITask task = queue->dequeue();
        }
    }
};
