#include "doctest.h"
#include "thread_safe_queue.h"

TEST_CASE("ThreadSafeQueue throws exception when empty") {
    ThreadSafeQueue<int> queue;
    CHECK_THROWS_AS(queue.dequeue(), QueueEmptyException);
}

TEST_CASE("ThreadSafeQueue")
