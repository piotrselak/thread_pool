#pragma once

#include <memory>
#include <thread_safe_queue.h>
#include <thread>
#include <functional>

#include "task.h"

// class IThreadPool {
// };

// TODO runtime management of adjusting threads/detecting ideal number
class Executor {
public:
    explicit Executor(const int thread_num = 4) : queue_(
        std::make_shared<ThreadSafeQueue<std::shared_ptr<ITask> > >()) {
        for (int i = 0; i < thread_num; i++)
            workers.emplace_back(worker_thread, this);
    };

    ~Executor() {
        shutdown_ = true;

        for (auto &worker: workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    // TODO rework to std::futures
    template<typename R, typename Func, typename... Args>
    std::shared_ptr<Task<R> > compute(Func &&f, Args &&... args) {
        std::function<R()> task_function = std::bind(f, args...);
        auto task = std::make_shared<Task<R> >(task_function);
        queue_->enqueue(task);
        return task;
    }

private:
    std::vector<std::thread> workers;
    std::shared_ptr<ThreadSafeQueue<std::shared_ptr<ITask> > > queue_;
    std::atomic<bool> shutdown_;

    // TODO for now its very simple
    // TODO condition_variable to run the loop stuff, not every ms
    // TODO handle exceptions so they do not kill threads
    void worker_thread() {
        while (!shutdown_) {
            if (queue_->is_empty()) continue;
            auto task = queue_->dequeue();
            task->execute();
            task->set_completed();
        }
    }
};
