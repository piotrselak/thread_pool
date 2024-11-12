#pragma once

#include <queue>
#include <mutex>
#include <stdexcept>

class QueueEmptyException final : public std::runtime_error {
public:
    QueueEmptyException(): runtime_error("ThreadSafeQueue is empty") {
    }
};

template<typename T>
class ThreadSafeQueue {
public:
    T head() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue_.front();
    }

    void enqueue(T item) {
        std::lock_guard<std::mutex> lock(mtx);
        queue_.push(item);
    }

    // Popped item is deleted, so if we want to use returned item
    // it has to be a shared pointer or something
    T dequeue() {
        std::lock_guard<std::mutex> lock(mtx);

        if (queue_.empty())
            throw QueueEmptyException();

        T popped_item = queue_.front();
        queue_.pop();
        return popped_item;
    }

    bool is_empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return queue_.empty();
    }

private:
    mutable std::mutex mtx;
    std::queue<T> queue_;
};


