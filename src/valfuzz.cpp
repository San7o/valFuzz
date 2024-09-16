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

auto &get_function_execute_before()
{
    static std::function<void()> function_execute_before = []() {};
    return function_execute_before;
}

auto &get_function_execute_after()
{
    static std::function<void()> function_execute_after = []() {};
    return function_execute_after;
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

auto &get_header()
{
     constinit static std::atomic<bool> header = true;
     return header;
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

void set_header(bool header)
{
    auto &header_ref = get_header();
    header_ref = header;
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

void parse_args(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--no-multithread")
        {
            set_multithreaded(false);
        }
        else if (std::string(argv[i]) == "--verbose")
        {
            set_verbose(true);
        }
        else if (std::string(argv[i]) == "--max-threads")
        {
            if (i + 1 < argc)
            {
                set_max_num_threads(std::stoul(argv[i + 1]));
                i++;
            }
        }
        else if (std::string(argv[i]) == "--no-header")
        {
            set_header(false);
        }
        else if (std::string(argv[i]) == "--help")
        {
            std::print("Usage: valfuzz [options]\n");
            std::print("Options:\n");
            std::print("  --no-multithread: run tests in a single thread\n");
            std::print("  --verbose: print test names\n");
            std::print("  --max-threads <num>: set the maximum number of threads\n");
            std::print("  --no-header: do not print the header at the start\n");
            std::print("  --help: print this help message\n");
            std::exit(0);
        }
        else
        {
            std::print("Unknown option: {}\n", argv[i]);
            std::exit(1);
        }
    }
}

char valfuzz_banner[] =
"             _ _____              \n"
" __   ____ _| |  ___|   _ ________\n"
" \\ \\ / / _` | | |_ | | | |_  /_  /\n"
"  \\ V / (_| | |  _|| |_| |/ / / / \n"
"   \\_/ \\__,_|_|_|   \\__,_/___/___|\n"
"                                  \n"
"A modern testing & fuzzing library for C++\n";

void print_header()
{
    bool verbose = get_verbose();
    bool is_threaded = get_is_threaded();
    long unsigned int max_num_threads = get_max_num_threads();
    std::lock_guard<std::mutex> lock(get_stream_mutex());
    std::print("{}", valfuzz_banner);
    std::print("Settings:\n");
    std::print(" - Multithreaded: {}\n", is_threaded);
    std::print(" - Max threads: {}\n", max_num_threads);
    std::print(" - Verbose: {}\n", verbose);
    std::print("\n");
}

} // namespace valfuzz

int main(int argc, char **argv)
{
    valfuzz::parse_args(argc, argv);
    if (valfuzz::get_header())
        valfuzz::print_header();

    valfuzz::get_function_execute_before()();
    valfuzz::run_tests();
    valfuzz::get_function_execute_after()();

    //  get_args(a_function);

    std::print("Tests finished\n");
    return 0;
}
