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

valFuzz (or val-di-Fuzz) is a modern cross-platform testing and fuzzing library for C++23. Check out [tests](./tests) for some examples on how to use this library.

## Features

- [x] assert macros

- [x] automatic test registrazion

- [x] parallel execution

- [x] arguments options

- [x] execute before / after all

- [x] fuzzing

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
  --test <name>: run a specific test
  --fuzz: run fuzz tests
  --fuzz-one <name>: run a specific fuzz test
  --no-multithread: run tests in a single thread
  --verbose: print test names
  --max-threads <num>: set the maximum number of threads
  --no-header: do not print the header at the start
  --help: print this help message
```

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

If you want, you can run a specific test by passing It's name to `--test` argument:
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
You can setup a fuzz test using the `FUZZME` macro, specifying am unique
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

You can run with fuzzing by specifyting `--fuzz`:
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

# License

This libray falls under [MIT](./LICENSE) license.
