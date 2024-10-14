#include "doctest.h"
#include "thread_safe_queue.h"
#include <chrono>
#include <thread>


TEST_CASE("ThreadSafeQueue throws exception when empty") {
    ThreadSafeQueue<int> queue;
    CHECK_THROWS_AS(queue.dequeue(), QueueEmptyException);
}

TEST_CASE("ThreadSafeQueue works correctly for concurrent operations") {
    ThreadSafeQueue<int> queue;

    SUBCASE("bugs shouldn't occur when two threads are pushing and popping from queue") {
        auto f = [&queue]() {
            for (int i = 0; i < 5000; i++)
                queue.enqueue(i);
            for (int i = 0; i < 5000; i++)
                queue.dequeue();
        };

        std::thread th1(f);
        std::thread th2(f);

        th1.join();
        th2.join();

        CHECK(queue.is_empty());
    }

    REQUIRE(queue.is_empty());

    SUBCASE("one thread should be adding new elements while other threads delete them when possible") {
        auto f1 = [&queue]() {
            for (int i = 0; i < 200; i++)
                queue.enqueue(i);
        };

        auto f2 = [&queue]() {
            while (!queue.is_empty()) {
                queue.dequeue();
            }
        };

        std::thread th1(f1);
        // TODO redo with condition variable
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::thread th2(f2);
        std::thread th3(f2);

        th1.join();
        th2.join();
        th3.join();

        CHECK(queue.is_empty());
    }

    SUBCASE("queue elements should keep proper order") {
    }
}

