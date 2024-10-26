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

#pragma once

#include <atomic>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <tuple>

#include "valfuzz/common.hpp"

namespace valfuzz
{

#define NUM_ITERATIONS_BENCHMARK 100000

/* Benchmarks */

#define BENCHMARK(name, pretty_name)                                           \
    void name([[maybe_unused]] const std::string &benchmark_name);             \
    static struct name##_register                                              \
    {                                                                          \
        name##_register()                                                      \
        {                                                                      \
            valfuzz::add_benchmark(pretty_name, name);                         \
        }                                                                      \
    } name##_register_instance;                                                \
    void name([[maybe_unused]] const std::string &benchmark_name)

#define RUN_BENCHMARK(input_size, ...)                                         \
    {                                                                          \
        std::cout << std::flush;                                               \
        std::chrono::duration<double> average =                                \
            std::chrono::duration<double>::zero();                             \
        for (int i = 0; i < valfuzz::get_num_iterations_benchmark(); i++)      \
        {                                                                      \
            auto start = std::chrono::high_resolution_clock::now();            \
            __VA_ARGS__;                                                       \
            auto end = std::chrono::high_resolution_clock::now();              \
            std::chrono::duration<double> elapsed = end - start;               \
            average += elapsed;                                                \
        }                                                                      \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cout << "benchmark: \"" << benchmark_name << "\", time: "         \
                  << average.count() / valfuzz::get_num_iterations_benchmark() \
                  << "s \n";                                                   \
        std::cout << std::flush;                                               \
        if (valfuzz::get_save_to_file())                                       \
        {                                                                      \
            valfuzz::get_save_file()                                           \
                << "\"" << benchmark_name << "\","                             \
                << average.count() / valfuzz::get_num_iterations_benchmark()   \
                << "," << input_size << "\n";                                  \
        }                                                                      \
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
