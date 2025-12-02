// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>

int sum_slow(int a, int b)
{
  int sum = 0;
  for (int i = 0; i < a; i++)
  {
    sum += b;
  }
  return sum;
}

BENCHMARK(bench_sum_slow, "Sum slow")
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

BENCHMARK(bench_sum_arrays, "Sum arrays")
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

BENCHMARK(bench_matrix_multiply, "Matrix multiply")
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

  RUN_BENCHMARK(100 * 100 * 100 * sizeof(float),
                matrix_multiply(M, N, P, 100, 100, 100));
}

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

BENCHMARK(bench_insertion_sort, "Insertion sort")
{
  int arr[10000];
  int out[10000];
  for (int i = 0; i < 10000; i++)
  {
    arr[i] = 10000 - i;
  }

  RUN_BENCHMARK(10 * sizeof(int), insertion_sort(arr, out, 10));
  RUN_BENCHMARK(100 * sizeof(int), insertion_sort(arr, out, 100));
  RUN_BENCHMARK(1000 * sizeof(int), insertion_sort(arr, out, 1000));
}

#ifdef openMP

void omp_array_sum(int *arr1, int *arr2, int *out, size_t N)
{
#pragma omp parallel for
  for (size_t i = 0; i < N; ++i)
  {
    out[i] = arr1[i] + arr2[i];
  }
}

BENCHMARK(bench_omp_array_sum, "Omp array sum")
{
#define OMP_ARRAY_SUM_SIZE 10000
  int arr1[OMP_ARRAY_SUM_SIZE];
  int arr2[OMP_ARRAY_SUM_SIZE];
  int out[OMP_ARRAY_SUM_SIZE];
  for (int i = 0; i < OMP_ARRAY_SUM_SIZE; i++)
  {
    arr1[i] = i;
    arr2[i] = 10000 - i;
    out[i] = 0;
  }

  omp_set_num_threads(1);
  RUN_BENCHMARK(10000 * sizeof(int), omp_array_sum(arr1, arr2, out, 10000));
  for (int i = 0; i < 10000; ++i)
  {
    if (out[i] != 10000)
    {
      std::cerr << "Benchmark: omp array sum: incorrect output\n";
      exit(1);
    }
    out[i] = 0;
  }

  omp_set_num_threads(4);
  RUN_BENCHMARK(10000 * sizeof(int), omp_array_sum(arr1, arr2, out, 10000));
  for (int i = 0; i < 10000; ++i)
  {
    if (out[i] != 10000)
    {
      std::cerr << "Benchmark: omp array sum: incorrect output\n";
      exit(1);
    }
    out[i] = 0;
  }

  omp_set_num_threads(8);
  RUN_BENCHMARK(10000 * sizeof(int), omp_array_sum(arr1, arr2, out, 10000));
  for (int i = 0; i < 10000; ++i)
  {
    if (out[i] != 10000)
    {
      std::cerr << "Benchmark: omp array sum: incorrect output\n";
      exit(1);
    }
    out[i] = 0;
  }
}
#endif
