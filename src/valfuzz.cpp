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

namespace valfuzz
{

std::deque<test_pair> &get_tests()
{
    static std::deque<test_pair> registered_tests = {};
    return registered_tests;
}

std::deque<fuzz_pair> &get_fuzzs()
{
    static std::deque<fuzz_pair> registered_fuzzs = {};
    return registered_fuzzs;
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

std::mutex &get_tests_mutex()
{
    constinit static std::mutex tests_mutex;
    return tests_mutex;
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

std::atomic<bool> &get_is_threaded()
{
    constinit static std::atomic<bool> is_threaded = true;
    return is_threaded;
}

std::vector<std::thread> &get_thread_pool()
{
    constinit static std::vector<std::thread> thread_pool;
    return thread_pool;
}

std::atomic<long unsigned int> &get_max_num_threads()
{
    constinit static std::atomic<long unsigned int> max_num_threads = 4;
    return max_num_threads;
}

std::atomic<bool> &get_verbose()
{
    constinit static std::atomic<bool> verbose = false;
    return verbose;
}

std::atomic<bool> &get_header()
{
    constinit static std::atomic<bool> header = true;
    return header;
}

std::atomic<bool> &get_do_fuzzing()
{
    constinit static std::atomic<bool> do_fuzzing = false;
    return do_fuzzing;
}

std::optional<std::string> &get_test_one()
{
    constinit static std::optional<std::string> test_one = std::nullopt;
    return test_one;
}

std::optional<std::string> &get_fuzz_one()
{
    constinit static std::optional<std::string> fuzz_one = std::nullopt;
    return fuzz_one;
}

long unsigned int get_num_fuzz_tests()
{
    auto &fuzzs = get_fuzzs();
    return fuzzs.size();
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

void set_do_fuzzing(bool do_fuzzing)
{
    auto &do_fuzzing_ref = get_do_fuzzing();
    do_fuzzing_ref = do_fuzzing;
}

void set_test_one(const std::string &test_one)
{
    auto &test_one_ref = get_test_one();
    test_one_ref = test_one;
}

void set_fuzz_one(const std::string &fuzz_one)
{
    auto &fuzz_one_ref = get_fuzz_one();
    fuzz_one_ref = fuzz_one;
}

void add_test(const std::string &name, test_function test)
{
    auto &tests = get_tests();
    std::lock_guard<std::mutex> lock(get_tests_mutex());
    tests.push_back({name, test});
}

void add_fuzz_test(const std::string &name, fuzz_function fuzz)
{
    auto &fuzzs = get_fuzzs();
    std::lock_guard<std::mutex> lock(get_tests_mutex());
    fuzzs.push_back({name, fuzz});
}

std::optional<test_pair> pop_test_or_null()
{
    auto &tests = get_tests();
    auto &tests_mutex = get_tests_mutex();

    std::lock_guard<std::mutex> lock(tests_mutex);
    if (tests.empty())
    {
        return std::nullopt;
    }
    test_pair test = tests.front();
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
                std::print("Running test: {}\n", test.first);
            }
            found = true;
            test.second(test.first);
            break;
        }
    }
    if (!found)
    {
        std::lock_guard<std::mutex> lock(get_stream_mutex());
        std::print("Test \"{}\" not found\n", name);
        std::exit(1);
    }
}

void _run_tests()
{
    auto test = std::optional<test_pair>{};
    while ((test = pop_test_or_null()).has_value())
    {
        // run task
        if (get_verbose())
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::print("Running test: \"{}\"\n", test.value().first);
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

std::atomic<long unsigned int> &get_iterations()
{
    constinit static std::atomic<long unsigned int> iterations = 0;
    return iterations;
}

void increment_iterations()
{
    auto &iterations = get_iterations();
    iterations++;
}

std::optional<fuzz_pair> pop_fuzz_or_null()
{
    auto &fuzzs = get_fuzzs();
    auto &fuzzs_mutex = get_tests_mutex();

    std::lock_guard<std::mutex> lock(fuzzs_mutex);
    if (fuzzs.empty())
    {
        return std::nullopt;
    }
    test_pair test = fuzzs.front();
    fuzzs.pop_front();
    return test;
}

void run_one_fuzz(const std::string &name)
{
    auto &fuzzs = get_fuzzs();
    std::deque<fuzz_pair> the_fuzz;
    std::for_each(fuzzs.begin(), fuzzs.end(),
                  [&the_fuzz, &name](fuzz_pair &fuzz)
                  {
                      if (fuzz.first == name)
                      {
                          for (long unsigned int i = 0;
                               i < get_max_num_threads(); i++)
                          {
                              the_fuzz.push_back(fuzz);
                          }
                      }
                  });
    if (the_fuzz.empty())
    {
        std::lock_guard<std::mutex> lock(get_stream_mutex());
        std::print("Fuzz test \"{}\" not found\n", name);
        std::exit(1);
    }
    fuzzs = the_fuzz;
    {
        std::lock_guard<std::mutex> lock(get_stream_mutex());
        std::print("Running fuzz test: {}\n", name);
    }
}

void _run_fuzz_tests()
{
    auto fuzz = std::optional<fuzz_pair>{};
    while ((fuzz = pop_fuzz_or_null()).has_value())
    {
        if (get_verbose())
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::print("Running fuzz: \"{}\"\n", fuzz.value().first);
        }
        fuzz.value().second(fuzz.value().first);

        increment_iterations();
        long unsigned int iterations = get_iterations();
        if (iterations % 1000000 == 0)
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::print("Iterations: {}\n", iterations);
        }

        add_fuzz_test(fuzz.value().first, fuzz.value().second);
    }
}

void run_fuzz_tests()
{
    if (get_is_threaded())
    {
        auto &thread_pool = get_thread_pool();
        // spawn threads
        for (long unsigned int i = 0;
             i < get_max_num_threads() && i < get_num_fuzz_tests(); i++)
        {
            thread_pool.push_back(std::thread(_run_fuzz_tests));
        }
        for (auto &thread : get_thread_pool())
        {
            thread.join();
        }
    }
    else
    {
        _run_fuzz_tests();
    }
}

void parse_args(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "--test")
        {
            if (i + 1 < argc)
            {
                set_test_one(argv[i + 1]);
                i++;
            }
        }
        else if (std::string(argv[i]) == "--fuzz")
        {
            set_do_fuzzing(true);
        }
        else if (std::string(argv[i]) == "--fuzz-one")
        {
            if (i + 1 < argc)
            {
                set_do_fuzzing(true);
                set_fuzz_one(argv[i + 1]);
                i++;
            }
        }
        else if (std::string(argv[i]) == "--benchmark")
        {
            set_do_benchmarks(true);
        }
        else if (std::string(argv[i]) == "--num-iterations")
        {
            if (i + 1 < argc)
            {
                set_num_iterations_benchmark(std::stoi(argv[i + 1]));
                i++;
            }
        }
        else if (std::string(argv[i]) == "--no-multithread")
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
            std::print("  --test <name>: run a specific test\n");
            std::print("  --fuzz: run fuzz tests\n");
            std::print("  --fuzz-one <name>: run a specific fuzz test\n");
            std::print("  --benchmark: run benchmarks\n");
            std::print("  --num-iterations <num>: set the number of "
                       "iterations for benchmarks\n");
            std::print("  --no-multithread: run tests in a single thread\n");
            std::print("  --verbose: print test names\n");
            std::print(
                "  --max-threads <num>: set the maximum number of threads\n");
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

char valfuzz_banner[] = "             _ _____              \n"
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
    bool do_fuzzing = get_do_fuzzing();
    bool do_benchmarks = get_do_benchmarks();
    long unsigned int max_num_threads = get_max_num_threads();
    std::lock_guard<std::mutex> lock(get_stream_mutex());
    std::print("{}", valfuzz_banner);
    std::print("Settings:\n");
    std::print(" - Multithreaded: {}\n", is_threaded);
    std::print(" - Max threads: {}\n", max_num_threads);
    std::print(" - Run Fuzzs: {}\n", do_fuzzing);
    std::print(" - Run Benchmarks: {}\n", do_benchmarks);
    std::print(" - Verbose: {}\n", verbose);
    std::print("\n");
}

template <> int get_random<int>()
{
    return std::rand();
}

template <> float get_random<float>()
{
    return static_cast<float>(std::rand());
}

template <> double get_random<double>()
{
    double dot =
        static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
    return static_cast<double>(std::rand()) + dot;
}

template <> char get_random<char>()
{
    return static_cast<char>(std::rand() % 256);
}

template <> bool get_random<bool>()
{
    return static_cast<bool>(std::rand() % 2);
}

template <> std::string get_random<std::string>()
{
    int len = std::rand() % MAX_RANDOM_STRING_LEN;
    std::string random_string = "";
    for (int i = 0; i < len; i++)
    {
        random_string += get_random<char>();
    }
    return random_string;
}

#if defined(_WIN32) || defined(_WIN64)
unsigned long get_cache_l3_size()
{
    return 0;
}
#else
unsigned long get_cache_l3_size()
{
    std::ifstream file("/sys/devices/system/cpu/cpu0/cache/index2/size");
    std::string size_str;
    if (file.is_open())
    {
        std::getline(file, size_str);
        file.close();
    }

    char unit = size_str.back();
    size_str.pop_back();
    unsigned long cache_size = std::stoul(size_str);

    switch (unit)
    {
    case 'K':
        cache_size *= 1024;
        break;
    case 'M':
        cache_size *= 1024 * 1024;
        break;
    default:
        break;
    }

    return cache_size;
}
#endif

bool &get_do_benchmarks()
{
    constinit static bool do_benchmarks = false;
    return do_benchmarks;
}

std::deque<benchmark_pair> &get_benchmarks()
{
    static std::deque<benchmark_pair> registered_benchmarks = {};
    return registered_benchmarks;
}

int &get_num_iterations_benchmark()
{
    constinit static int num_iterations_benchmark = 100000;
    return num_iterations_benchmark;
}

unsigned long get_num_benchmarks()
{
    auto &benchmarks = get_benchmarks();
    return benchmarks.size();
}

void add_benchmark(const std::string &name, benchmark_function benchmark)
{
    auto &benchmarks = get_benchmarks();
    benchmarks.push_back({name, benchmark});
}

void set_do_benchmarks(bool do_benchmarks)
{
    auto &do_benchmarks_ref = get_do_benchmarks();
    do_benchmarks_ref = do_benchmarks;
}

void set_num_iterations_benchmark(int num_iterations)
{
    auto &num_iterations_benchmark = get_num_iterations_benchmark();
    num_iterations_benchmark = num_iterations;
}

void run_benchmarks()
{
    const size_t bigger_than_cachesize = get_cache_l3_size() * 2;
    long *p;
    if (bigger_than_cachesize != 0)
        p = new long[bigger_than_cachesize];
    {
        std::lock_guard<std::mutex> lock(get_stream_mutex());
        std::print("Cache size: {}\n", get_cache_l3_size());
    }
    for (auto &benchmark : get_benchmarks())
    {
        // flush cache
        for (size_t i = 0; i < bigger_than_cachesize; i++)
        {
            p[i] = std::rand();
        }
        if (get_verbose())
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::print("Running benchmark: {}\n", benchmark.first);
            std::cout << std::flush;
        }
        else
        // this is needed or the benchmark will totally be shit, don't ask me
        // why
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::print("");
            std::cout << std::flush;
        }
        benchmark.second(benchmark.first);
    }
    if (bigger_than_cachesize != 0)
        delete[] p;
}

} // namespace valfuzz

int main(int argc, char **argv)
{
    valfuzz::parse_args(argc, argv);
    if (valfuzz::get_header())
        valfuzz::print_header();

    unsigned int seed = (unsigned int) std::time(nullptr);
    std::srand(seed);

    valfuzz::get_function_execute_before()();

    if (valfuzz::get_do_benchmarks())
    {
        {
            std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
            std::print("Running {} benchmarks...\n",
                       valfuzz::get_num_benchmarks());
        }
        valfuzz::run_benchmarks();
    }
    else if (!valfuzz::get_do_fuzzing())
    {
        if (valfuzz::get_test_one().has_value())
        {
            valfuzz::run_one_test(valfuzz::get_test_one().value());
        }
        else
        {
            {
                std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
                std::print("Seed: {}\n", seed);
                std::print("Running {} tests...\n", valfuzz::get_num_tests());
            }
            valfuzz::run_tests();
        }
    }
    else
    {
        if (valfuzz::get_fuzz_one().has_value())
        {
            valfuzz::run_one_fuzz(valfuzz::get_fuzz_one().value());
        }
        else
        {
            std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
            std::print("Seed: {}\n", seed);
            std::print("Running {} fuzz tests...\n",
                       valfuzz::get_num_fuzz_tests());
        }
        valfuzz::run_fuzz_tests();
    }

    valfuzz::get_function_execute_after()();
    std::print("Done\n");

    return 0;
}
