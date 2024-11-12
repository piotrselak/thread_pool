#pragma once

#include <memory>
#include <thread_safe_queue.h>
#include <thread>
#include <functional>
#include <future>


// TODO runtime management of adjusting threads/detecting ideal number
class Executor {
public:
    explicit Executor(const int thread_num = 4) {
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

    template<typename R, typename Func, typename... Args>
    std::future<R> compute(Func &&f, Args &&... args) {
        std::packaged_task<R()> pt(std::bind(std::forward<Func>(f), std::forward<Args>(args)...));
        std::future<R> result = pt.get_future();

        queue_.enqueue([task = std::make_shared<std::packaged_task<R()> >(std::move(pt))]() { (*task)(); });
        return result;
    }

private:
    std::vector<std::thread> workers;

    ThreadSafeQueue<std::function<void()> > queue_;

    std::atomic<bool> shutdown_;

    // TODO for now its very simple
    // TODO condition_variable to run the loop stuff, not every ms and signal end
    // TODO handle exceptions so they do not kill threads
    void worker_thread() {
        while (!shutdown_) {
            if (queue_.is_empty()) continue;
            if (auto task = queue_.dequeue()) {
                task();
            }
        }
    }
};
