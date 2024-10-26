/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

/*
 * Benchmarks for google benchmark library to compare with valfuzz.
 */

#include <benchmark/benchmark.h>

int sum_slow(int a, int b)
{
    int sum = 0;
    for (int i = 0; i < a; i++)
    {
        sum += b;
    }
    return sum;
}

static void BM_sum_slow(benchmark::State &state)
{
    int a = 10000;
    int b = 20000;
    for (auto _ : state)
    {
        sum_slow(a, b);
    }
}

BENCHMARK(BM_sum_slow);

void sum_arr(int a[], int b[], int n, int *sum)
{
    *sum = 0;
    for (int i = 0; i < n; i++)
    {
        *sum += a[i] + b[i];
    }
}

static void BM_sum_arrays(benchmark::State &state)
{
    int a[10000];
    int b[10000];
    for (int i = 0; i < 10000; i++)
    {
        a[i] = i;
        b[i] = i;
    }
    int sum = 0;
    for (auto _ : state)
    {
        sum_arr(a, b, 10000, &sum);
    }
}

BENCHMARK(BM_sum_arrays);

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

static void BM_matrix_multiply(benchmark::State &state)
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

    for (auto _ : state)
    {
        matrix_multiply(M, N, P, 100, 100, 100);
    }
}

BENCHMARK(BM_matrix_multiply);

void insertion_sort(int in[], int out[], int n)
{
    for (int i = 0; i < n; i++)
    {
        int j = i;
        while (j > 0 && out[j - 1] > in[i])
        {
            out[j] = out[j - 1];
            j--;
        }
        out[j] = in[i];
    }
}

static void BM_insertion_sort(benchmark::State &state)
{
    int arr[1000];
    int out[1000];
    for (int i = 0; i < 1000; i++)
    {
        arr[i] = 1000 - i;
    }

    for (auto _ : state)
    {
        insertion_sort(arr, out, 1000);
    }
}

BENCHMARK(BM_insertion_sort);

BENCHMARK_MAIN();
