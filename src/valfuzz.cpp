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

#include "valfuzz/valfuzz.hpp"

#include <print>

namespace valfuzz
{

auto &get_tests()
{
    static std::deque<test_pair> registered_tests = {};
    return registered_tests;
}

auto &get_tests_queue()
{
    static std::deque<test_pair> tests_queue = {};
    return tests_queue;
}

std::mutex &get_tests_queue_mutex()
{
    constinit static std::mutex tests_queue_mutex;
    return tests_queue_mutex;
}

std::mutex &get_stream_mutex()
{
    constinit static std::mutex stream_mutex;
    return stream_mutex;
}

long unsigned int get_num_tests()
{
    auto &tests = get_tests();
    return tests.size();
}

auto &get_is_threaded()
{
    constinit static std::atomic<bool> is_threaded = true;
    return is_threaded;
}

auto &get_thread_pool()
{
    constinit static std::vector<std::thread> thread_pool;
    return thread_pool;
}

auto &get_max_num_threads()
{
    constinit static std::atomic<long unsigned int> max_num_threads = 4;
    return max_num_threads;
}

auto &get_verbose()
{
    constinit static std::atomic<bool> verbose = false;
    return verbose;
}

void set_multithreaded(bool is_threaded)
{
    auto &is_threaded_ref = get_is_threaded();
    is_threaded_ref = is_threaded;
}

void set_max_num_threads(long unsigned int max_num_threads)
{
    auto &max_num_threads_ref = get_max_num_threads();
    max_num_threads_ref = max_num_threads;
}

void set_verbose(bool verbose)
{
    auto &verbose_ref = get_verbose();
    verbose_ref = verbose;
}

void add_test(const std::string &name, test_function test)
{
    auto &tests = get_tests();
    tests.push_back({name, test});
}

void add_test_to_queue(const std::string &name, test_function test)
{
    auto &tests_queue = get_tests_queue();
    auto &tests_queue_mutex = get_tests_queue_mutex();

    std::lock_guard<std::mutex> lock(tests_queue_mutex);
    tests_queue.push_back({name, test});
}

std::optional<test_pair> pop_test_from_queue_or_null()
{
    auto &tests_queue = get_tests_queue();
    auto &tests_queue_mutex = get_tests_queue_mutex();

    std::lock_guard<std::mutex> lock(tests_queue_mutex);
    if (tests_queue.empty())
    {
        return std::nullopt;
    }
    test_pair test = tests_queue.front();
    tests_queue.pop_front();
    return test;
}

void run_test_parallel()
{
    auto test = std::optional<test_pair>{};
    while ((test = std::move(pop_test_from_queue_or_null())).has_value())
    {
        // run task
        if (get_verbose())
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::print("Running test: {}\n", test.value().first);
        }
        test.value().second(test.value().first);
    }
}

void run_tests()
{
    auto &tests = get_tests();

    if (get_is_threaded())
    {
        // load tests queue
        std::for_each(tests.begin(), tests.end(), [](auto &test)
                      { add_test_to_queue(test.first, test.second); });

        // spawn threads
        for (long unsigned int i = 0; i < get_max_num_threads() && i < get_num_tests(); i++)
        {
            auto &thread_pool = get_thread_pool();
            thread_pool.push_back(std::thread(run_test_parallel));
        }
        for (auto &thread : get_thread_pool())
        {
            thread.join();
        }
    }
    else
    {
        std::for_each(tests.begin(), tests.end(), [](auto &test)
                      { test.second(test.first); });
    }
}

} // namespace valfuzz

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    // TODO: parse args

    std::print("Starting tests, running {} tests\n", valfuzz::get_num_tests());

    valfuzz::set_multithreaded(true); // default true
    valfuzz::set_verbose(false); // default false
    valfuzz::run_tests();

    //  get_args(a_function);

    std::print("Tests finished\n");
    return 0;
}
