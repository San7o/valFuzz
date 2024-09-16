# valFuzz

```
             _ _____
 __   ____ _| |  ___|   _ ________
 \ \ / / _` | | |_ | | | |_  /_  /
  \ V / (_| | |  _|| |_| |/ / / /
   \_/ \__,_|_|_|   \__,_/___/___|

A modern testing & fuzzing framework for C++
Settings:
 - Multithreaded: true
 - Max threads: 4
 - Verbose: true

Running test: Simple Assertion
...
```

valFuzz (or val-di-Fuzz) is a modern cross-platform testing and fuzzing library for c++23.

## Features

- [x] assert macros

- [x] tests

- [x] parallel execution

- [x] toggle verbose

- [x] arguments settings

- [ ] execute before / after all

- [ ] naive fuzzing

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

## Examples

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
