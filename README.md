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

Seed: 1726579491
Running 1337 tests...
```

valFuzz (or val-di-Fuzz) is a modern cross-platform testing, fuzzing and benchmarking library for C++17/20/23. Check out [tests](./tests) for some examples on how to use this library, or read the online documentation [here](https://san7o.github.io/brenta-engine-documentation/valfuzz/v1.0/).

> valFuzz is the official testing library for [Brenta Engine](https://github.com/San7o/Brenta-Engine).

## Features

- [x] assert macros

- [x] automatic test registration

- [x] parallel execution

- [x] arguments options

- [x] execute before / after all

- [x] fuzzing

- [x] benchmarking

# Usage
```
Usage: valfuzz [options]
Options:
  --test <name>: run a specific test
  --fuzz: run fuzz tests
  --fuzz-one <name>: run a specific fuzz test
  --benchmark: run benchmarks
  --num-iterations <num>: set the number of iterations for benchmarks
  --no-multithread: run tests in a single thread
  --run-one-benchmark <name>: run a specific benchmark
  --verbose: print test names
  --max-threads <num>: set the maximum number of threads
  --no-header: do not print the header at the start
  --help: print this help message
```

Copy [include/valfuzz/valfuzz.hpp](./include/valfuzz/valfuzz.hpp) and [src/valfuzz.cpp](./src/valfuzz.cpp)
in your include and source directories respectively, or link to the shared library. You can compile
a shared or static library with the following flags:

```bash
cmake -Bbuild -DVALFUZZ_BUILD_SHARED=ON -DVALFUZZ_BUILD_STATIC=ON
```

### bazel (experimental)
```
bazel build //src:libvalfuzz --sandbox_debug --verbose_failures
```

### cpm
You can easily use [cpm](https://github.com/cpm-cmake/CPM.cmake) to include
the library in your project.
```
CPMAddPackage(
    NAME valfuzz
    GITHUB_REPOSITORY San7o/valFuzz
    GIT_TAG v1.0.4
    DOWNLOAD_ONLY True
)
if (valfuzz_ADDED AND BRENTA_BUILD_TESTS)
    list(APPEND MY_INCLUDES ${valfuzz_SOURCE_DIR}/include)
    list(APPEND MY_SOURCES ${valfuzz_SOURCE_DIR}/src/valfuzz.cpp)
endif()
```

### meson
```
[wrap-git]
url = https://github.com/San7o/valFuzz
revision = v1.0.4
depth = 1
```

## Documentation
You can read a comprehensive documentation [here](https://san7o.github.io/brenta-engine-documentation/valfuzz/v1.0/). Here is presented a quick guide to showcase the library's api.

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

The library already contains a main so you just need to define the tests with `TEST`, the first
argument is the test name, the second one is the name that will be displayed in the output
when an assertion on the test fails.

## Run a single test

If you want, you can run a specific test by passing Its name to `--test` argument:
```bash
./build/asserts_test --test "Simple Assertion"
```

## Execute before and after all
You can set a function to be executed either before or after all the tests with
the macro `BEFORE()` and `AFTER()`:
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
You can set up a fuzz test using the `FUZZME` macro, specifying an unique
fuzz name and a string name:
```c++
FUZZME(simple_fuzzing, "Simple fuzzing")
{
    int a = valfuzz::get_random<int>();
    int b = valfuzz::get_random<int>();
    int ret = sum_int(a, b);
    ASSERT_EQ(ret, a + b);
}
```
and use the `get_random<T>()` function to get a random value of `T` type. Fuzz
tests are executed continuously in a multithreaded environment (unless you
specify `--no-multithread`) until you stop the program.

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

You can define a benchmark function with the macro `BENCHMARK`. Inside the benchmark, you can
do any setup / cleanup and run an expression to be benchmarked with `BENCHMARK_RUN`.

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

BENCHMARK(bench_sum_slow, "Sum slow benchmark")
{
    int a = 1;
    int b = 2;
    RUN_BENCHMARK(sum_slow(a, b));
}
```

Compile and run with `--benchmark` flag:
```bash
./build/valfuzz --benchmark --verbose
```
```
Running benchmark: Sum slow benchmark
benchmark: "Sum slow benchmark", time: 1.6342204000065603e-08s
```

The benchmark will be run 100000 times to get the average time. You
can set the number of iterations using the `--num-iterations` flag.

# License

This library falls under [MIT](./LICENSE) license.
