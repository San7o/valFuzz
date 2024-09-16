#include "valfuzz/valfuzz.hpp"
/*
FUZZME(a_function, int a, int b)
{
    [[maybe_unused]] auto c = a + b;
    return 0;
}
*/

int sum_int(int a, int b)
{
    return a + b;
}

TEST(naive_fuzzing, "Naive fuzzing")
{
    int a = valfuzz::get_random<int>();
    int b = valfuzz::get_random<int>();
    int ret = sum_int(a, b);
    ASSERT_EQ(ret, a + b);
}
