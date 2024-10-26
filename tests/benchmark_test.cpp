#include "valfuzz/valfuzz.hpp"

int sum_slow(int a, int b)
{
    int sum = 0;
    for (int i = 0; i < a; i++)
    {
        sum += b;
    }
    return sum;
}

BENCHMARK(bench_sum_slow, "Sum slow benchmark")
{
    int a = 10000;
    int b = 20000;
    RUN_BENCHMARK(a, sum_slow(a, b));
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
    // RUN_BENCHMARK(100 * sizeof(int), sum_arr(a, b, 100, &sum));
    // RUN_BENCHMARK(1000 * sizeof(int), sum_arr(a, b, 1000, &sum));
    RUN_BENCHMARK(10000 * sizeof(int), sum_arr(a, b, 10000, &sum));
}

void matrix_multiply(float **M, float **N, float **P, int m, int n, int p)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < p; j++)
        {
            P[i][j] = 0;
            for (int k = 0; k < n; k++)
            {
                P[i][j] += M[i][k] * N[k][j];
            }
        }
    }
}

BENCHMARK(bench_matrix_multiply, "Matrix multiply benchmark")
{
    float **M = new float *[100];
    float **N = new float *[100];
    float **P = new float *[100];
    for (int i = 0; i < 100; i++)
    {
        M[i] = new float[100];
        N[i] = new float[100];
        P[i] = new float[100];
        for (int j = 0; j < 100; j++)
        {
            M[i][j] = static_cast<float>(i + j); 
            N[i][j] = static_cast<float>(i - j);
        }
    }

    RUN_BENCHMARK(100 * 100 * 100 * sizeof(float), matrix_multiply(M, N, P, 100, 100, 100));
}

void insertion_sort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

BENCHMARK(bench_insertion_sort, "Insertion sort benchmark")
{
    int arr[10000];
    for (int i = 0; i < 10000; i++)
    {
        arr[i] = 10000 - i;
    }

    RUN_BENCHMARK(10000 * sizeof(int), insertion_sort(arr, 10000));
}
