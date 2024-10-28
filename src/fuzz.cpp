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

#include "valfuzz/fuzz.hpp"

namespace valfuzz
{

std::mt19937 &get_random_engine()
{
    static std::mt19937 random_engine;
    return random_engine;
}

std::uniform_real_distribution<> &get_uniform_distribution()
{
    static std::uniform_real_distribution<> distribution(-1.0, 1.0);
    return distribution;
}

template <> __attribute__((noinline)) int get_random<int>()
{
    return std::rand();
}

template <> float get_random<float>()
{
    std::mt19937 &random_engine = valfuzz::get_random_engine();
    std::uniform_real_distribution<> &uniform_distribution =
        valfuzz::get_uniform_distribution();
    return static_cast<float>(uniform_distribution(random_engine));
}

template <> double get_random<double>()
{
    std::mt19937 &random_engine = valfuzz::get_random_engine();
    std::uniform_real_distribution<> &uniform_distribution =
        valfuzz::get_uniform_distribution();
    return static_cast<double>(uniform_distribution(random_engine));
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

std::deque<fuzz_pair> &get_fuzzs()
{
    static std::deque<fuzz_pair> registered_fuzzs = {};
    return registered_fuzzs;
}

long unsigned int get_num_fuzz_tests()
{
    auto &fuzzs = get_fuzzs();
    return fuzzs.size();
}

std::atomic<long unsigned int> &get_iterations()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static std::atomic<long unsigned int>
            iterations = 0;
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
    valfuzz::test_pair test = fuzzs.front();
    fuzzs.pop_front();
    return test;
}

void add_fuzz_test(const std::string &name, fuzz_function fuzz)
{
    auto &fuzzs = get_fuzzs();
    std::lock_guard<std::mutex> lock(get_tests_mutex());
    fuzzs.push_back({name, fuzz});
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
        std::cout << "Fuzz test \"" << name << "\" not found\n";
        std::exit(1);
    }
    fuzzs = the_fuzz;
    {
        std::lock_guard<std::mutex> lock(get_stream_mutex());
        std::cout << "Running fuzz test: " << name << "\n";
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
            std::cout << "Running fuzz: \"" << fuzz.value().first << "\"\n";
        }
        fuzz.value().second(fuzz.value().first);

        increment_iterations();
        long unsigned int iterations = get_iterations();
        if (iterations % 1000000 == 0)
        {
            std::lock_guard<std::mutex> lock(get_stream_mutex());
            std::cout << "Iterations: " << iterations << "\n";
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

} // namespace valfuzz
