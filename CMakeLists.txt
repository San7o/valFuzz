cmake_minimum_required(VERSION 3.29.6)
project("valFuzz")

set(CMAKE_CXX_STANDARD 23 REQUIRED)

option(VALFUZZ_BUILD_EXAMPLES "Build examples" ON)
option(VALFUZZ_BUILD_TESTS "Build tests" ON)
option(VALFUZZ_BUILD_SHARED "Build shared library" OFF)
option(VALFUZZ_BUILD_STATIC "Build static library" OFF)

set(VALFUZZ_EXAMPLES_SOURCE)
set(VALFUZZ_INCLUDES)
set(VALFUZZ_SOURCES)
set(VALFUZZ_COMPILE_OPTIONS)
set(VALFUZZ_TEST_SOURCES)

list(APPEND VALFUZZ_INCLUDES include)
list(APPEND VALFUZZ_SOURCES src/valfuzz.cpp)
list(APPEND VALFUZZ_COMPILE_OPTIONS -Wall -Wextra -Werror -Wpedantic)
list(APPEND VALFUZZ_TEST_SOURCES
    tests/asserts_test.cpp
    tests/before_after_test.cpp
)

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

if (VALFUZZ_BUILD_EXAMPLES)
    add_executable(valfuzz_simple ${VALFUZZ_SOURCES} ${VALFUZZ_EXAMPLES_SOURCE})
    target_include_directories(valfuzz_simple PRIVATE ${VALFUZZ_INCLUDES})
    target_compile_options(valfuzz_simple PRIVATE ${VALFUZZ_COMPILE_OPTIONS})
endif()
if (VALFUZZ_BUILD_TESTS)
    add_executable(asserts_test ${VALFUZZ_SOURCES} ${VALFUZZ_TEST_SOURCES})
    target_include_directories(asserts_test PRIVATE ${VALFUZZ_INCLUDES})
    target_compile_options(asserts_test PRIVATE ${VALFUZZ_COMPILE_OPTIONS})
endif()

