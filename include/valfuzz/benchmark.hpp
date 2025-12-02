// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <atomic>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <mutex>
#include <optional>
#include <string>
#include <tuple>
#include <valfuzz/common.hpp>
#include <valfuzz/reporter.hpp>

#ifdef openMP
#include <omp.h>
#endif

namespace valfuzz
{

#define NUM_ITERATIONS_BENCHMARK 10000

/* Benchmarks */

#define BENCHMARK(name, pretty_name)                                           \
  void name([[maybe_unused]] const std::string &benchmark_name);               \
  static struct name##_register                                                \
  {                                                                            \
    name##_register()                                                          \
    {                                                                          \
      valfuzz::add_benchmark(pretty_name, name);                               \
    }                                                                          \
  } name##_register_instance;                                                  \
  void name([[maybe_unused]] const std::string &benchmark_name)

#define RUN_BENCHMARK(input_size, ...)                                         \
  {                                                                            \
    std::cout << std::flush;                                                   \
    std::chrono::duration<double> total =                                      \
      std::chrono::duration<double>::zero();                                   \
    double mean = 0.0;                                                         \
    double M2 = 0.0;                                                           \
    const int N_ITER = valfuzz::get_num_iterations_benchmark();                \
    double *times = new double[N_ITER];                                        \
    /* run twice to warm up the cache */                                       \
    __VA_ARGS__;                                                               \
    __VA_ARGS__;                                                               \
    for (int i = 0; i < N_ITER; i++)                                           \
    {                                                                          \
      auto start = std::chrono::high_resolution_clock::now();                  \
      __VA_ARGS__;                                                             \
      auto end = std::chrono::high_resolution_clock::now();                    \
      std::chrono::duration<double> elapsed = end - start;                     \
      total += elapsed;                                                        \
      double e = elapsed.count();                                              \
      times[i] = e;                                                            \
      /* Welford's algorithm  */                                               \
      double delta = e - mean;                                                 \
      mean += delta / (i + 1);                                                 \
      M2 += (e - mean) * delta;                                                \
    }                                                                          \
    double variance = M2 / N_ITER;                                             \
    std::sort(times, times + N_ITER);                                          \
    struct valfuzz::report rep = {                                             \
      benchmark_name,                                                          \
      (long unsigned int) input_size,                                          \
      times[N_ITER > 1 ? 1 : 0],                                               \
      times[N_ITER > 1 ? N_ITER - 2 : N_ITER - 1],                             \
      times[N_ITER / 2],                                                       \
      total.count() / N_ITER,                                                  \
      std::sqrt(variance),                                                     \
      times[N_ITER / 4],                                                       \
      times[3 * N_ITER / 4],                                                   \
    };                                                                         \
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());             \
    std::cout                                                                  \
      << valfuzz::reporter_eg.report(&rep, valfuzz::get_reporter()).str()      \
      << std::flush;                                                           \
    if (valfuzz::get_save_to_file())                                           \
    {                                                                          \
      valfuzz::get_save_file()                                                 \
        << valfuzz::reporter_eg.report(&rep, valfuzz::get_reporter()).str()    \
        << std::flush;                                                         \
    }                                                                          \
  }

typedef std::function<void(std::string)> benchmark_function;
typedef std::pair<std::string, benchmark_function> benchmark_pair;

unsigned long get_cache_l3_size();
bool &get_do_benchmarks();
int &get_num_iterations_benchmark();
bool &get_run_one_benchmark();
std::string &get_one_benchmark();
unsigned long get_num_benchmarks();
std::deque<benchmark_pair> &get_benchmarks();
std::atomic<bool> &get_save_to_file();
std::ofstream &get_save_file();

void set_save_to_file(bool save_to_file);
void set_save_file(const std::filesystem::path &save_to_file_path);
void add_benchmark(const std::string &name, benchmark_function benchmark);
void set_do_benchmarks(bool do_benchmarks);
void set_num_iterations_benchmark(int num_iterations_benchmark);
void set_run_one_benchmark(bool run_one_benchmark);
void set_one_benchmark(const std::string &one_benchmark);

void run_benchmarks();

} // namespace valfuzz
