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
    RUN_BENCHMARK(1, sum_fast(a, b));
}

BENCHMARK(bench_sum_slow, "Sum slow benchmark")
{
    int a = 1;
    int b = 2;
    RUN_BENCHMARK(1, sum_slow(a, b));
}

BENCHMARK(bench_sum_fast2, "Sum fast benchmark2")
{
    int a = 1;
    int b = 2;
    RUN_BENCHMARK(1, sum_fast(a, b));
}

BENCHMARK(bench_sum_slow2, "Sum slow benchmark2")
{
    int a = 1;
    int b = 2;
    RUN_BENCHMARK(1, sum_slow(a, b));
}

void sum_arr(int a[], int b[], int n, int *sum)
{
    *sum = 0;
    for (int i = 0; i < n; i++)
    {
        *sum += a[i] + b[i];
    }
}

BENCHMARK(bench_sum_arrays, "Sum arrays benchmark")
{
    int a[10000];
    int b[10000];
    for (int i = 0; i < 10000; i++)
    {
        a[i] = i;
        b[i] = i;
    }
    int sum = 0;
    RUN_BENCHMARK(100 * sizeof(int), sum_arr(a, b, 100, &sum));
    RUN_BENCHMARK(1000 * sizeof(int), sum_arr(a, b, 1000, &sum));
    RUN_BENCHMARK(10000 * sizeof(int), sum_arr(a, b, 10000, &sum));
}
