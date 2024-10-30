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

#include "valfuzz/benchmark.hpp"

namespace valfuzz
{

std::atomic<bool> &get_save_to_file()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static std::atomic<bool>
            save_to_file = false;
    return save_to_file;
}

#if defined(__linux__)
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
#elif defined(_WIN32) || defined(_WIN64)
unsigned long get_cache_l3_size()
{
    return 0;
}
#else
unsigned long get_cache_l3_size()
{
    return 0;
}
#endif

bool &get_do_benchmarks()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static bool do_benchmarks = false;
    return do_benchmarks;
}

std::deque<benchmark_pair> &get_benchmarks()
{
    static std::deque<benchmark_pair> registered_benchmarks = {};
    return registered_benchmarks;
}

int &get_num_iterations_benchmark()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static int num_iterations_benchmark = 100000;
    return num_iterations_benchmark;
}

unsigned long get_num_benchmarks()
{
    auto &benchmarks = get_benchmarks();
    return benchmarks.size();
}

bool &get_run_one_benchmark()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static bool run_one_benchmark = false;
    return run_one_benchmark;
}

std::string &get_one_benchmark()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static std::string one_benchmark = "";
    return one_benchmark;
}

std::ofstream &get_save_file()
{
    static std::ofstream output;
    return output;
}

void set_save_to_file(bool save_to_file)
{
    auto &save_to_file_ref = get_save_to_file();
    save_to_file_ref = save_to_file;
}

void set_save_file(const std::filesystem::path &output_dir)
{
    auto &output_dir_ref = get_save_file();
    output_dir_ref = std::ofstream(output_dir);
    if (!output_dir_ref.is_open())
    {
        std::lock_guard<std::mutex> lock(get_stream_mutex());
        std::cout << "Could not open file " << output_dir << "\n";
        std::exit(1);
    }
    output_dir_ref << "name,space,min,max,median,mean,sd,q1,q3\n";
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

void set_run_one_benchmark(bool run_one_benchmark)
{
    auto &run_one_benchmark_ref = get_run_one_benchmark();
    run_one_benchmark_ref = run_one_benchmark;
}

void set_one_benchmark(const std::string &one_benchmark)
{
    auto &one_benchmark_ref = get_one_benchmark();
    one_benchmark_ref = one_benchmark;
}

void run_benchmarks()
{
    const size_t bigger_than_cachesize = get_cache_l3_size() * 2;
    long *p;
    if (bigger_than_cachesize != 0)
        p = new long[bigger_than_cachesize];
    {
        std::lock_guard<std::mutex> lock(get_stream_mutex());
        std::cout << "Cache size: " << get_cache_l3_size() << "\n";
    }
    for (auto &benchmark : get_benchmarks())
    {
        // flush cache
        for (size_t i = 0; i < bigger_than_cachesize; i++)
        {
            p[i] = std::rand();
        }

        if (get_run_one_benchmark())
        {
            if (benchmark.first != get_one_benchmark())
                continue;
        }

        if (get_verbose())
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::cout << "Running benchmark: " << benchmark.first << "\n";
            std::cout << std::flush;
        }
        else
        // this is needed or the benchmark will totally be shit, don't ask me
        // why
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::cout << std::flush;
        }
        benchmark.second(benchmark.first);
    }
    if (bigger_than_cachesize != 0)
        delete[] p;
}

} // namespace valfuzz
