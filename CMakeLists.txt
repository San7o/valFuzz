cmake_minimum_required(VERSION 3.29.6)
project("valFuzz")

set(CMAKE_CXX_STANDARD 23 REQUIRED)

option(VALFUZZ_USE_CLANG "Use clang compiler" OFF)
option(VALFUZZ_BUILD_TESTS "Build tests" ON)
option(VALFUZZ_BUILD_SHARED "Build shared library" OFF)
option(VALFUZZ_BUILD_STATIC "Build static library" OFF)
option(VALFUZZ_BUILD_CLOCK_PRECISION "Build an executable to
                          get the best clock's precision" OFF)

set(VALFUZZ_INCLUDES)
set(VALFUZZ_SOURCES)
set(VALFUZZ_COMPILE_OPTIONS)
set(VALFUZZ_TEST_SOURCES)

list(APPEND VALFUZZ_INCLUDES include)
list(APPEND VALFUZZ_SOURCES src/valfuzz.cpp)
list(APPEND VALFUZZ_COMPILE_OPTIONS -Wall -Wextra -Werror -Wpedantic -Wconversion)
list(APPEND VALFUZZ_TEST_SOURCES
    tests/asserts_test.cpp
    tests/before_after_test.cpp
    tests/simple_fuzz.cpp
    tests/generate_random_test.cpp
    tests/benchmark_test.cpp
)

if (VALFUZZ_USE_CLANG)
    set(CMAKE_CXX_COMPILER clang++)
endif()

if (VALFUZZ_BUILD_EXAMPLES)
    list(APPEND VALFUZZ_EXAMPLES_SOURCE
        examples/simple_fuzz.cpp)
endif()

if (VALFUZZ_BUILD_SHARED)
    add_library(valfuzz_shared SHARED ${VALFUZZ_SOURCES})
    target_include_directories(valfuzz_shared PRIVATE ${VALFUZZ_INCLUDES})
    target_compile_options(valfuzz_shared PRIVATE ${VALFUZZ_COMPILE_OPTIONS})
endif()
if (VALFUZZ_BUILD_STATIC)
    add_library(valfuzz_static STATIC ${VALFUZZ_SOURCES})
    target_include_directories(valfuzz_static PRIVATE ${VALFUZZ_INCLUDES})
    target_compile_options(valfuzz_static PRIVATE ${VALFUZZ_COMPILE_OPTIONS})
endif()
if (VALFUZZ_BUILD_TESTS)
    add_executable(asserts_test ${VALFUZZ_SOURCES} ${VALFUZZ_TEST_SOURCES})
    target_include_directories(asserts_test PRIVATE ${VALFUZZ_INCLUDES})
    target_compile_options(asserts_test PRIVATE ${VALFUZZ_COMPILE_OPTIONS})
endif()
if (VALFUZZ_BUILD_CLOCK_PRECISION)
    add_executable(clock_precision utils/clock_precision.cpp)
    target_compile_options(clock_precision PRIVATE ${VALFUZZ_COMPILE_OPTIONS})
endif()
