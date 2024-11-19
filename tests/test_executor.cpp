#include "doctest.h"
#include "executor.h"


TEST_CASE("Executor executes tasks") {
    Executor ex;
    auto sum = [](const int a, const int b) { return a + b; };

    auto result1 = ex.compute<int>(sum, 2, 3);
    auto result2 = ex.compute<int>(sum, 2, 2);

    CHECK_EQ(result1.get(), 5);
    CHECK_EQ(result2.get(), 4);
}
