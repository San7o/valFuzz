```
             _ _____
 __   ____ _| |  ___|   _ ________
 \ \ / / _` | | |_ | | | |_  /_  /
  \ V / (_| | |  _|| |_| |/ / / /
   \_/ \__,_|_|_|   \__,_/___/___|

A modern testing & fuzzing library for C++
Settings:
 - Multithreaded: true
 - Max threads: 4
 - Verbose: true
 - Reporter: default

Seed: 1726579491
Running 1337 tests...
```

valFuzz is a modern cross-platform testing, fuzzing and benchmarking
library for C++17 onwards.  Check out [tests](./tests) for some
examples on how to use this library, or read the online documentation
[here](https://san7o.github.io/brenta-engine-documentation/valfuzz/v1.0/).

# Usage

```
Usage: valfuzz [options]
Options:

 TESTS
  --test <name>: run a specific test
  --no-multithread: run tests in a single thread
  --max-threads <num>: set the maximum number of threads

 FUZZING
  --fuzz: run fuzz tests
  --fuzz-one <name>: run a specific fuzz test

 BENCHMARK
  --benchmark: run benchmarks
  --num-iterations <num>: set the number of iterations for benchmarks
  --run-one-benchmark <name>: run a specific benchmark
  --report <file>: save benchmark results to a file
  --reporter <name>: use a custom reporter, currently supported
                     are default, csv, none

 GENERAL
  --verbose: print test names
  --no-header: do not print the header at the start
  --seed <seed>: set the seed for PRNG
  --help: print this help message
```

## Compile shared library

### cmake

```bash
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build -j 4
```

### bazel

```
bazel build //:libvalfuzz
```

### cpm

You can easily use [cpm](https://github.com/cpm-cmake/CPM.cmake) to include
the library in your project.
```
CPMAddPackage(
    NAME valfuzz
    GITHUB_REPOSITORY San7o/valFuzz
    GIT_TAG v1.1.0
    DOWNLOAD_ONLY True
)
```

### meson

```
[wrap-git]
url = https://github.com/San7o/valFuzz
revision = v1.1.0
depth = 1
```

# Documentation

You can read a comprehensive documentation
[here](https://san7o.github.io/brenta-engine-documentation/valfuzz/v1.0/).
Here is presented a quick guide to showcase the library's api.

## Run tests

You can run a test by defining it with the `TEST` macro:

```c++
#include "valfuzz/valfuzz.hpp"

TEST(simple, "Simple Assertion") {
    ASSERT(1 == 2);
}
```

```
test: Simple Assertion, line: 3, Assertion failed: 1 != 2
```

The library already contains a main so you just need to define the
tests with `TEST`, the first argument is the test name, the second one
is the name that will be displayed in the output when an assertion on
the test fails.

## Run a single test

If you want, you can run a specific test by passing Its name to
`--test` argument:

```bash
./build/asserts_test --test "Simple Assertion"
```

## Execute before and after all

You can set a function to be executed either before or after all the
tests with the macro `BEFORE()` and `AFTER()`:

```c++
BEFORE() {
    std::cout << "Before test" << std::endl;
}

AFTER() {
    std::cout << "After test" << std::endl;
}

TEST(a_test, "Another simple test") {
    ASSERT_EQ(1, 1);
}
```

## Fuzzing

You can set up a fuzz test using the `FUZZME` macro, specifying an
unique fuzz name and a string name:

```c++
FUZZME(simple_fuzzing, "Simple fuzzing")
{
    int a = valfuzz::get_random<int>();
    int b = valfuzz::get_random<int>();
    int ret = sum_int(a, b);
    ASSERT_EQ(ret, a + b);
}
```

and use the `get_random<T>()` function to get a random value of `T`
type. Fuzz tests are executed continuously in a multithreaded
environment (unless you specify `--no-multithread`) until you stop the
program.

You can run with fuzzing by specifying `--fuzz`:

```bash
./build/asserts_test --fuzz
```

You can also specify a specific fuzz test to fuzz:

```bash
./build/asserts_test --fuzz-one "Simple fuzzing"
```

```
Running fuzz test: Simple fuzzing
Iterations: 1000000
Iterations: 2000000
Iterations: 3000000
...
```

## Benchmarks

You can define a benchmark function with the macro `BENCHMARK`.
Inside the benchmark, you can do any setup / cleanup and run an
expression to be benchmarked with `BENCHMARK_RUN(space, expr)`.  The
first argument is gonna be the space complecity, this will be used to
produce a report, the second argument is the expression that will be
evaluated. You can specify a reporter or provide one your own since It
very easy. Yon can save the reports in a file via `--report
<file>`. If you didn't specify a report file, you can set any value in
the first argument.

> currently, the cache will be reset between benchmarks only on linux

For example:

```c++
int sum_slow(int a, int b)
{
    int sum = 0;
    for (int i = 0; i < a; i++)
    {
        sum += b;
    }
    return sum;
}

BENCHMARK(bench_sum_slow, "Sum slow")
{
    int a = 1000;
    int b = 2000;
    RUN_BENCHMARK(1, sum_slow(a, b));
}
```

Compile and run with `--benchmark` flag:

```bash
./build/valfuzz --benchmark --verbose
```

```
Running benchmark: Sum slow
benchmark: "Sum slow"
 - space: 10000
 - min: 1.7114e-05s
 - max: 2.9317e-05s
 - mean: 1.94025e-05s
 - standard deviation: 1.63826e-06
 - Q1: 1.8365e-05s
 - Q3: 2.0794e-05s
```

The benchmark will be run 10000 times by default. You can set the
number of iterations using the `--num-iterations` flag.  If you
specified `--reporter csv`, a csv output will be generated with the
following structure:

```
benchmark_name,space_complexity,min_time,max_time,mean,standard_deviation,quantile1,quantile3
```

You can quickly generate a graph with python by following the instructions
in [plotting/README.md](plotting/README.md).

![image](https://github.com/user-attachments/assets/0cae5078-2bf3-481d-a2e8-b822f2918bfb)

# License

This library falls under [MIT](./LICENSE) license.
