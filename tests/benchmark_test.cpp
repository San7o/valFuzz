#include "valfuzz/valfuzz.hpp"

int sum_fast(int a, int b)
{
    return a + b;
}

int sum_slow(int a, int b)
{
    int sum = 0;
    for (int i = 0; i < a; i++)
    {
        sum += b;
    }
    return sum;
}

BENCHMARK(bench_sum_fast, "Sum fast benchmark")
{
    int a = 1;
    int b = 2;
    RUN_BENCHMARK(sum_fast(a, b));
}

BENCHMARK(bench_sum_slow, "Sum slow benchmark")
{
    int a = 1;
    int b = 2;
    RUN_BENCHMARK(sum_slow(a, b));
}

BENCHMARK(bench_sum_fast2, "Sum fast benchmark2")
{
    int a = 1;
    int b = 2;
    RUN_BENCHMARK(sum_fast(a, b));
}

BENCHMARK(bench_sum_slow2, "Sum slow benchmark2")
{
    int a = 1;
    int b = 2;
    RUN_BENCHMARK(sum_slow(a, b));
}
