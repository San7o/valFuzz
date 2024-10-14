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

#include <algorithm>
#include <any>
#include <atomic>
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
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>

#define NUM_ITERATIONS_BENCHMARK 100000

namespace valfuzz
{

/* Assertions */

#define ASSERT(cond)                                                           \
    if (!(cond))                                                               \
    {                                                                          \
        valfuzz::set_has_failed_once(true);                                    \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #cond << std::endl;               \
    }

#define ASSERT_EQ(a, b)                                                        \
    if ((a) != (b))                                                            \
    {                                                                          \
        valfuzz::set_has_failed_once(true);                                    \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " != " << #b << std::endl;  \
    }

#define ASSERT_NE(a, b)                                                        \
    if ((a) == (b))                                                            \
    {                                                                          \
        valfuzz::set_has_failed_once(true);                                    \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " == " << #b << std::endl;  \
    }

#define ASSERT_LT(a, b)                                                        \
    if ((a) >= (b))                                                            \
    {                                                                          \
        valfuzz::set_has_failed_once(true);                                    \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " < " << #b << std::endl;   \
    }

#define ASSERT_LE(a, b)                                                        \
    if ((a) > (b))                                                             \
    {                                                                          \
        valfuzz::set_has_failed_once(true);                                    \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " <= " << #b << std::endl;  \
    }

#define ASSERT_GT(a, b)                                                        \
    if ((a) <= (b))                                                            \
    {                                                                          \
        valfuzz::set_has_failed_once(true);                                    \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " > " << #b << std::endl;   \
    }

#define ASSERT_GE(a, b)                                                        \
    if ((a) < (b))                                                             \
    {                                                                          \
        valfuzz::set_has_failed_once(true);                                    \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " >= " << #b << std::endl;  \
    }

#define ASSERT_THROW(expr, exception)                                          \
    {                                                                          \
        bool exception_thrown = false;                                         \
        try                                                                    \
        {                                                                      \
            expr;                                                              \
        }                                                                      \
        catch (const exception &e)                                             \
        {                                                                      \
            exception_thrown = true;                                           \
        }                                                                      \
        if (!exception_thrown)                                                 \
        {                                                                      \
            valfuzz::set_has_failed_once(true);                                \
            std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());     \
            std::cerr << "test: " << test_name << ", line: " << __LINE__       \
                      << ", ";                                                 \
            std::cerr << "Exception not thrown: " << #exception << std::endl;  \
        }                                                                      \
    }

#define ASSERT_NO_THROW(expr)                                                  \
    {                                                                          \
        try                                                                    \
        {                                                                      \
            expr;                                                              \
        }                                                                      \
        catch (...)                                                            \
        {                                                                      \
            valfuzz::set_has_failed_once(true);                                \
            std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());     \
            std::cerr << "test: " << test_name << ", line: " << __LINE__       \
                      << ", ";                                                 \
            std::cerr << "Exception thrown" << std::endl;                      \
        }                                                                      \
    }

std::mutex &get_stream_mutex();
std::atomic<long unsigned int> &get_max_num_threads();
std::atomic<bool> &get_is_threaded();
std::atomic<bool> &get_verbose();
std::atomic<bool> &get_header();
std::atomic<bool> &get_do_fuzzing();
std::optional<std::string> &get_test_one();
std::optional<std::string> &get_fuzz_one();

void set_multithreaded(bool is_threaded);
void set_max_num_threads(long unsigned int max_num_threads);
void set_verbose(bool verbose);
void set_header(bool header);
void set_do_fuzzing(bool do_fuzzing);
void set_test_one(const std::string &test_one);
void set_fuzz_one(const std::string &fuzz_one);

void parse_args(int argc, char *argv[]);
void print_header();

/* Tests */

typedef std::function<void(std::string)> test_function;
typedef std::pair<std::string, test_function> test_pair;

std::deque<test_pair> &get_tests();
std::vector<std::thread> &get_thread_pool();
std::mutex &get_test_mutex();
long unsigned int get_num_tests();
std::atomic<bool> &get_has_failed_once();

std::function<void()> &get_function_execute_before();
std::function<void()> &get_function_execute_after();

void set_function_execute_before(std::function<void()> f);
void set_function_execute_after(std::function<void()> f);
void set_has_failed_once(bool has_failed_once);

void add_test(const std::string &name, test_function test);

std::optional<test_pair> pop_test_or_null();
void run_one_test(const std::string &name);
void _run_tests();
void run_tests();

#define TEST(name, pretty_name)                                                \
    void name([[maybe_unused]] const std::string &test_name);                  \
    static struct name##_register                                              \
    {                                                                          \
        name##_register()                                                      \
        {                                                                      \
            valfuzz::add_test(pretty_name, name);                              \
        }                                                                      \
    } name##_register_instance;                                                \
    void name([[maybe_unused]] const std::string &test_name)

#define BEFORE()                                                               \
    void before();                                                             \
    static struct before##_register                                            \
    {                                                                          \
        before##_register()                                                    \
        {                                                                      \
            valfuzz::set_function_execute_before(before);                      \
        }                                                                      \
    } before##_register_instance;                                              \
    void before()

#define AFTER()                                                                \
    void after();                                                              \
    static struct after##_register                                             \
    {                                                                          \
        after##_register()                                                     \
        {                                                                      \
            valfuzz::set_function_execute_after(after);                        \
        }                                                                      \
    } after##_register_instance;                                               \
    void after()

/* Fuzzer */

#define MAX_RANDOM_STRING_LEN 1024

template <typename T> T get_random();

#define FUZZME(fun_name, pretty_name)                                          \
    void fun_name([[maybe_unused]] const std::string &test_name);              \
    static struct fun_name##_register                                          \
    {                                                                          \
        fun_name##_register()                                                  \
        {                                                                      \
            valfuzz::add_fuzz_test(pretty_name, fun_name);                     \
        }                                                                      \
    } fun_name##_register_instance;                                            \
    void fun_name([[maybe_unused]] const std::string &test_name)

typedef std::function<void(std::string)> fuzz_function;
typedef std::pair<std::string, fuzz_function> fuzz_pair;

std::deque<fuzz_pair> &get_fuzz_tests();
std::atomic<long unsigned int> &get_iterations();
long unsigned int get_num_fuzz_tests();
std::atomic<bool> &get_save_to_file();
std::ofstream &get_save_file();

void set_save_to_file(bool save_to_file);
void set_save_file(const std::filesystem::path &save_to_file_path);

void increment_iterations();
std::optional<fuzz_pair> pop_fuzz_or_null();
void add_fuzz_test(const std::string &name, fuzz_function test);
void run_one_fuzz(const std::string &name);
void _run_fuzz_tests();
void run_fuzz_tests();

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

// TODO: incremental iterations
#define RUN_BENCHMARK(input_size, ...)                                                     \
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
                << "," << input_size << "\n";                                                       \
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

void add_benchmark(const std::string &name, benchmark_function benchmark);
void set_do_benchmarks(bool do_benchmarks);
void set_num_iterations_benchmark(int num_iterations_benchmark);
void set_run_one_benchmark(bool run_one_benchmark);
void set_one_benchmark(const std::string &one_benchmark);

void run_benchmarks();

} // namespace valfuzz
