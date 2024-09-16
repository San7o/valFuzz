# valFuzz

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

Running 1337 tests...
```

valFuzz (or val-di-Fuzz) is a modern cross-platform testing and fuzzing library for c++23.

## Features

- [x] assert macros

- [x] tests

- [x] parallel execution

- [x] toggle verbose

- [x] arguments settings

- [x] execute before / after all

- [x] naive fuzzing

- [ ] genetic fuzzing

# Usage

Copy [include/valfuzz/valfuzz.hpp](./include/valfuzz/valfuzz.hpp) and [src/valfuzz.cpp](./src/valfuzz.cpp)
in your include and source directories respectively, or link to the shared library. You can compile
a shared or static library with the following flags:

```bash
cmake -Bbuild -DVALFUZZ_BUILD_SHARED=ON -DVALFUZZ_BUILD_STATIC=ON
```

## Arguments
You can pass the following arguments to the compiled executable:
```
Usage: valfuzz [options]
Options:
  --no-multithread: run tests in a single thread
  --verbose: print test names
  --max-threads <num>: set the maximum number of threads
  --no-header: do not print the header at the start
  --help: print this help message
```

## Run a Test
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
The fizzer is not yet implemented, for now 
you can generate random number of basic types `int`, `double`, `float`, `char` and `string`:
```c++
int a = valfuzz::get_random<int>();
int b = valfuzz::get_random<int>();
int ret = sum_int(a, b);
ASSERT_EQ(ret, a + b);
```
