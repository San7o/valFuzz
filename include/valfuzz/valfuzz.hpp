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
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <print>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <utility>
#include <vector>
#include <limits>

namespace valfuzz
{

/* Assertions */

#define ASSERT(cond)                                                           \
    if (!(cond))                                                               \
    {                                                                          \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #cond << std::endl;               \
    }

#define ASSERT_EQ(a, b)                                                        \
    if ((a) != (b))                                                            \
    {                                                                          \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " != " << #b << std::endl;  \
    }

#define ASSERT_NE(a, b)                                                        \
    if ((a) == (b))                                                            \
    {                                                                          \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " == " << #b << std::endl;  \
    }

#define ASSERT_LT(a, b)                                                        \
    if ((a) >= (b))                                                            \
    {                                                                          \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " < " << #b << std::endl;   \
    }

#define ASSERT_LE(a, b)                                                        \
    if ((a) > (b))                                                             \
    {                                                                          \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " <= " << #b << std::endl;  \
    }

#define ASSERT_GT(a, b)                                                        \
    if ((a) <= (b))                                                            \
    {                                                                          \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " > " << #b << std::endl;   \
    }

#define ASSERT_GE(a, b)                                                        \
    if ((a) < (b))                                                             \
    {                                                                          \
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());         \
        std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";  \
        std::cerr << "Assertion failed: " << #a << " >= " << #b << std::endl;  \
    }

/* Tests */

typedef std::function<void(std::string)> test_function;
typedef std::function<void(std::string)> fuzz_function;
typedef std::pair<std::string, test_function> test_pair;
typedef std::pair<std::string, fuzz_function> fuzz_pair;

void parse_args(int argc, char *argv[]);
void print_header();

/* state */
std::deque<test_pair> &get_tests();
std::deque<fuzz_pair> &get_fuzz_tests();
std::deque<test_pair> &get_tests_queue();
std::vector<std::thread> &get_thread_pool();
std::function<void()> &get_function_execute_before();
std::function<void()> &get_function_execute_after();
std::mutex &get_test_queue_mutex();
std::mutex &get_stream_mutex();
std::atomic<long unsigned int> &get_max_num_threads();
std::atomic<bool> &get_is_threaded();
std::atomic<bool> &get_verbose();
std::atomic<bool> &get_header();

void set_multithreaded(bool is_threaded);
void set_max_num_threads(long unsigned int max_num_threads);
void set_verbose(bool verbose);
void set_header(bool header);
void set_function_execute_before(std::function<void()> f);
void set_function_execute_after(std::function<void()> f);

void add_test(const std::string &name, test_function test);
void add_fuzz_test(const std::string &name, fuzz_function test);
void add_test_to_queue(const std::string &name, test_function test);

std::optional<test_pair> pop_test_from_queue_or_null();
void run_test_parallel();
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

template <typename T> T get_random()
{
    return T();
}

// TODO

/*
#define FUZZME(fun_name, ...)                                                  \
    template <typename... Args> int fun_name(Args... args)                     \
    {                                                                          \
        return 0;                                                              \
    }                                                                          \
    static struct fun_name##_register                                          \
    {                                                                          \
        fun_name##_register()                                                  \
        {                                                                      \
            valfuzz::add_fuzz_test(#fun_name, fun_name);                       \
        }                                                                      \
    } fun_name##_register_instance;                                            \
    template <> int fun_name(__VA_ARGS__)

template <typename T> void process_type(std::vector<std::any> a_vec)
{
    T t = T();
    a_vec.push_back(t);
}

template <typename...> void process_types(std::vector<std::any> &)
{
}

template <typename T, typename... Rest>
void process_types(std::vector<std::any> &a_vec)
{
    process_type<T>(a_vec);
    process_types<Rest...>(a_vec);
}

template <typename... Args> void get_args(int (*f)(Args...))
{
    std::vector<std::any> a_vec;
    process_types<Args...>(a_vec);

    // debug printing
    for (const auto &t : a_vec)
    {
        std::cout << t.type().name() << " ";
    }

    // TODO:
    // Generate random values for each base type

    // call the function with the generated values

}
*/

} // namespace valfuzz
