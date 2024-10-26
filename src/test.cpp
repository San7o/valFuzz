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

#include "valfuzz/test.hpp"

namespace valfuzz
{

std::deque<valfuzz::test_pair> &get_tests()
{
    static std::deque<valfuzz::test_pair> registered_tests = {};
    return registered_tests;
}

long unsigned int get_num_tests()
{
    auto &tests = get_tests();
    return tests.size();
}

std::atomic<bool> &get_has_failed_once()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static std::atomic<bool>
            has_failed_once = false;
    return has_failed_once;
}

std::function<void()> &get_function_execute_before()
{
    static std::function<void()> function_execute_before = []() {};
    return function_execute_before;
}

std::function<void()> &get_function_execute_after()
{
    static std::function<void()> function_execute_after = []() {};
    return function_execute_after;
}

void set_function_execute_before(std::function<void()> f)
{
    auto &function_execute_before = get_function_execute_before();
    function_execute_before = f;
}

void set_function_execute_after(std::function<void()> f)
{
    auto &function_execute_after = get_function_execute_after();
    function_execute_after = f;
}

void set_has_failed_once(bool has_failed_once)
{
    auto &has_failed_once_ref = get_has_failed_once();
    has_failed_once_ref = has_failed_once;
}

void add_test(const std::string &name, test_function test)
{
    auto &tests = get_tests();
    std::lock_guard<std::mutex> lock(get_tests_mutex());
    tests.push_back({name, test});
}

std::optional<valfuzz::test_pair> pop_test_or_null()
{
    auto &tests = get_tests();
    auto &tests_mutex = get_tests_mutex();

    std::lock_guard<std::mutex> lock(tests_mutex);
    if (tests.empty())
    {
        return std::nullopt;
    }
    valfuzz::test_pair test = tests.front();
    tests.pop_front();
    return test;
}

void run_one_test(const std::string &name)
{
    auto &tests = get_tests();
    bool found = false;
    for (auto &test : tests)
    {
        if (test.first == name)
        {
            {
                std::lock_guard<std::mutex> lock(get_stream_mutex());
                std::cout << "Running test: " << test.first << "\n";
            }
            found = true;
            test.second(test.first);
            break;
        }
    }
    if (!found)
    {
        std::lock_guard<std::mutex> lock(get_stream_mutex());
        std::cout << "Test \"" << name << "\" not found\n";
        std::exit(1);
    }
}

void _run_tests()
{
    auto test = std::optional<valfuzz::test_pair>{};
    while ((test = pop_test_or_null()).has_value())
    {
        // run task
        if (get_verbose())
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::cout << "Running test: \"" << test.value().first << "\"\n";
        }
        test.value().second(test.value().first);
    }
}

void run_tests()
{
    if (get_is_threaded())
    {
        auto &thread_pool = get_thread_pool();
        // spawn threads
        for (long unsigned int i = 0;
             i < get_max_num_threads() && i < get_num_tests(); i++)
        {
            thread_pool.push_back(std::thread(_run_tests));
        }
        for (auto &thread : get_thread_pool())
        {
            thread.join();
        }
    }
    else
    {
        _run_tests();
    }
}


} // namespace valfuzz
