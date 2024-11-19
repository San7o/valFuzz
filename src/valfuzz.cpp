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

#include <random>

namespace valfuzz
{

std::atomic<bool> &get_header()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static std::atomic<bool>
            header = true;
    return header;
}

std::atomic<bool> &get_do_fuzzing()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static std::atomic<bool>
            do_fuzzing = false;
    return do_fuzzing;
}

std::optional<std::string> &get_test_one()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static std::optional<std::string>
            test_one = std::nullopt;
    return test_one;
}

std::optional<std::string> &get_fuzz_one()
{
#if __cplusplus >= 202002L // C++20
    constinit
#endif
        static std::optional<std::string>
            fuzz_one = std::nullopt;
    return fuzz_one;
}

std::atomic<long unsigned int> &get_seed()
{
    static std::atomic<long unsigned int> seed =
        (long unsigned int) std::time(nullptr);
    return seed;
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

void set_seed(long unsigned int new_seed)
{
    auto &seed = get_seed();
    seed = new_seed;
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
    std::string reporter_id = get_reporter();
    std::lock_guard<std::mutex> lock(get_stream_mutex());
    std::cout << valfuzz_banner;
    std::cout << "Settings:\n";
    std::cout << " - Multithreaded: " << is_threaded << "\n";
    std::cout << " - Max threads: " << max_num_threads << "\n";
    std::cout << " - Run Fuzzs: " << do_fuzzing << "\n";
    std::cout << " - Run Benchmarks: " << do_benchmarks << "\n";
    std::cout << " - Verbose: " << verbose << "\n";
    std::cout << " - Reporter: " << reporter_id << "\n";
    std::cout << "\n";
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
            else
            {
                std::cerr << "Test name not provided\n";
                std::exit(1);
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
            else
            {
                std::cerr << "Fuzz test name not provided\n";
                std::exit(1);
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
            else
            {
                std::cerr << "Number of iterations not provided\n";
                std::exit(1);
            }
        }
        else if (std::string(argv[i]) == "--run-one-benchmark")
        {
            if (i + 1 < argc)
            {
                bool found = false;
                for (auto b : get_benchmarks())
                {
                    if (b.first == argv[i + 1])
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    std::cerr << "Benchmark \"" << argv[i + 1]
                              << "\" not found\n";
                    std::exit(1);
                }
                set_do_benchmarks(true);
                set_run_one_benchmark(true);
                set_one_benchmark(argv[i + 1]);
                i++;
            }
        }
        else if (std::string(argv[i]) == "--report")
        {
            set_save_to_file(true);
            if (i + 1 < argc)
            {
                set_save_file(argv[i + 1]);
                i++;
            }
            else
            {
                std::cerr << "Report file not provided\n";
                std::exit(1);
            }
        }
        else if (std::string(argv[i]) == "--reporter")
        {
	  if (i + 1 < argc && reporter_eg.has(argv[i+1]))
            {
	      set_reporter(argv[i+1]);
                i++;
            }
            else
            {
	      std::cerr << "Reporter "<< argv[i+1]<< " unrecognised\n";
                std::exit(1);
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
            else
            {
                std::cerr << "Number of threads not provided\n";
                std::exit(1);
            }
        }
        else if (std::string(argv[i]) == "--no-header")
        {
            set_header(false);
        }
        else if (std::string(argv[i]) == "--seed")
        {
            if (i + 1 < argc)
            {
                set_seed(std::stoul(argv[i + 1]));
                i++;
            }
            else
            {
                std::cerr << "Seed not provided\n";
                std::exit(1);
            }
        }
        else if (std::string(argv[i]) == "--help")
        {
            std::cout << valfuzz_banner << "\n";
            std::cout << "Usage: valfuzz [options]\n";
            std::cout << "Options:\n";
            std::cout << "\n";
            std::cout << " TESTS \n";
            std::cout << "  --test <name>: run a specific test\n";
            std::cout << "  --no-multithread: run tests in a single thread\n";
            std::cout
                << "  --max-threads <num>: set the maximum number of threads\n";
            std::cout << "\n";
            std::cout << " FUZZING \n";
            std::cout << "  --fuzz: run fuzz tests\n";
            std::cout << "  --fuzz-one <name>: run a specific fuzz test\n";
            std::cout << "\n";
            std::cout << " BENCHMARK \n";
            std::cout << "  --benchmark: run benchmarks\n";
            std::cout << "  --num-iterations <num>: set the number of "
                         "iterations for benchmarks\n";
            std::cout
                << "  --run-one-benchmark <name>: run a specific benchmark\n";
            std::cout
                << "  --report <file>: save benchmark results to a file\n";
            std::cout
                << "  --reporter <name>: use a custom reporter, currently supported\n";
	    std::cout
	        << "                     are default, csv, none\n"; 
            std::cout << "\n";
            std::cout << " GENERAL \n";
            std::cout << "  --verbose: print test names\n";
            std::cout
                << "  --no-header: do not print the header at the start\n";
            std::cout << "  --seed <seed>: set the seed for PRNG\n";
            std::cout << "  --help: print this help message\n";
            std::exit(0);
        }
        else
        {
            std::cerr << "Unknown option: " << argv[i] << "\n";
            std::exit(1);
        }
    }
}

} // namespace valfuzz

int main(int argc, char **argv)
{
    valfuzz::parse_args(argc, argv);
    if (valfuzz::get_header())
        valfuzz::print_header();

    auto &seed = valfuzz::get_seed();
    std::srand((unsigned int) seed);

    std::mt19937 gen = valfuzz::get_random_engine();
    gen.seed(seed.load());

    valfuzz::get_function_execute_before()();

    if (valfuzz::get_do_benchmarks())
    {
        {
            std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
            std::cout << "Seed: " << seed << "\n";
            std::cout << "Running " << valfuzz::get_num_benchmarks()
                      << " benchmarks...\n";
        }
        valfuzz::run_benchmarks();
        if (valfuzz::get_save_to_file())
        {
            std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
            std::cout << "Saved results to file \n";
            valfuzz::get_save_file().close();
        }
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
                std::cout << "Seed: " << seed << "\n";
                std::cout << "Running " << valfuzz::get_num_tests()
                          << " tests...\n";
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
            std::cout << "Seed: " << seed << "\n";
            std::cout << "Running " << valfuzz::get_num_fuzz_tests()
                      << " fuzz tests...\n";
        }
        valfuzz::run_fuzz_tests();
    }

    valfuzz::get_function_execute_after()();

    if (valfuzz::get_has_failed_once())
    {
        std::cerr << "Failed\n";
        return 1;
    }
    else
    {
        std::cout << "Done\n";
    }

    return 0;
}
