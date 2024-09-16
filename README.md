# valFuzz

valFuzz (or val-di-Fuzz) is a modern cross-platform testing and fuzzing library for c++23.

## Features

- [x] assert macros

- [x] tests

- [x] parallel execution

- [x] toggle verbose

- [ ] arguments settings

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
