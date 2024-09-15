cmake_minimum_required(VERSION 3.29.6)
project("brenta test")

set(CMAKE_CXX_STANDARD 23 REQUIRED)

option(BRENTA_BUILD_EXAMPLES "Build examples" ON)
option(BRENTA_BUILD_TESTS "Build tests" ON)

set(BRENTA_EXAMPLES_SOURCE)
set(BRENTA_INCLUDES)
set(BRENTA_SOURCES)

list(APPEND BRENTA_INCLUDES include)
list(APPEND BRENTA_SOURCES src/test.cpp)

if (BRENTA_BUILD_EXAMPLES)
    list(APPEND BRENTA_EXAMPLES_SOURCE
        examples/simple_fuzz.cpp)
endif()

if (BRENTA_BUILD_EXAMPLES)
    add_executable(brenta_test ${BRENTA_SOURCES} ${BRENTA_EXAMPLES_SOURCE})
    target_include_directories(brenta_test PRIVATE ${BRENTA_INCLUDES})
endif()
if (BRENTA_BUILD_TESTS)
    add_executable(asserts_test tests/asserts_test.cpp ${BRENTA_SOURCES})
    target_include_directories(asserts_test PRIVATE ${BRENTA_INCLUDES})
endif()

