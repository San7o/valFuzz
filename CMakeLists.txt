cmake_minimum_required(VERSION 3.16)
project(valfuzz
  VERSION 1.2.0
  DESCRIPTION "Modern testing, fuzzing and benchmarking library"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # no gnu extensions

#
# Options
#

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(VALFUZZ_USE_CLANG "Use clang compiler" OFF)
option(VALFUZZ_ENABLE_OPENMP "Enable OpenMP support" ON)
option(VALFUZZ_BUILD_TESTS "Build tests" OFF)
option(VALFUZZ_BUILD_CLOCK_PRECISION "Build an executable to
                          get the best clock's precision" OFF)
option(VALFUZZ_BUILD_NO_OPTIMIZED "Build without optimizations" OFF)
option(VALFUZZ_BUILD_OPTIMIZED_O1 "Build with optimizations -O1" OFF)
option(VALFUZZ_BUILD_OPTIMIZED_O2 "Build with optimizations -O2" OFF)
option(VALFUZZ_BUILD_OPTIMIZED "Build with optimizations -O3" OFF)
option(VALFUZZ_BUILD_OPTIMIZED_AGGRESSIVE "Build with aggressive optimizations" OFF)

#
# Sources and headers
#

set(VALFUZZ_INCLUDES)
set(VALFUZZ_SOURCES)
set(VALFUZZ_COMPILE_OPTIONS)
set(VALFUZZ_TEST_SOURCES)
set(VALFUZZ_LINK_OPTIONS pthread)

list(APPEND VALFUZZ_INCLUDES include)
file(GLOB VALFUZZ_SOURCES
  src/*.cpp)

list(APPEND VALFUZZ_COMPILE_OPTIONS
  -Wall -Wextra -Werror -Wpedantic -Wconversion)
file(GLOB VALFUZZ_TEST_SOURCES
  tests/*.cpp)

if (VALFUZZ_USE_CLANG)
  message("Using clang compiler")
  set(CMAKE_CXX_COMPILER clang++)
endif()

if (VALFUZZ_BUILD_EXAMPLES)
  message("Building with examples")
  list(APPEND VALFUZZ_EXAMPLES_SOURCE
    examples/simple_fuzz.cpp)
endif()

if (VALFUZZ_ENABLE_OPENMP)
  find_package(OpenMP)
  if(OpenMP_CXX_FOUND)
    message("Compiling with OpenMP")
    list(APPEND VALFUZZ_LINK_OPTIONS OpenMP::OpenMP_CXX)
    list(APPEND VALFUZZ_COMPILE_OPTIONS -D openMP)
  else()
    message(ERROR "OpenMP not found in your system")
  endif()
endif()

if (VALFUZZ_BUILD_OPTIMIZED_AGGRESSIVE)
  message("Building with aggressive optimizations: -O3 -march=native -Ofast")
  list(APPEND VALFUZZ_COMPILE_OPTIONS -O3 -march=native -Ofast)
elseif(VALFUZZ_BUILD_OPTIMIZED)
  message("Building with optimizations: -O3")
  list(APPEND VALFUZZ_COMPILE_OPTIONS -O3)
elseif(VALFUZZ_BUILD_OPTIMIZED_O2)
  message("Building with optimizations: -O2")
  list(APPEND VALFUZZ_COMPILE_OPTIONS -O2)
elseif(VALFUZZ_BUILD_OPTIMIZED_O1)
  message("Building with optimizations: -O1")
  list(APPEND VALFUZZ_COMPILE_OPTIONS -O1)
elseif(VALFUZZ_BUILD_NO_OPTIMIZED)
  message("Building without optimizations")
  list(APPEND VALFUZZ_COMPILE_OPTIONS -O0)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
  message("Building in Release mode")
  message(STATUS "Release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
else()
  message("Building in Debug mode")
  message(STATUS "Debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
endif()

#
# Targets
#

if (BUILD_SHARED_LIBS)
  message("Building shared library")
  add_library(${PROJECT_NAME} SHARED
    ${VALFUZZ_SOURCES})
else()
  message("Building static library")
  add_library(${PROJECT_NAME} STATIC
    ${VALFUZZ_SOURCES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  ${VALFUZZ_INCLUDES})
target_compile_options(${PROJECT_NAME} PRIVATE
  ${VALFUZZ_COMPILE_OPTIONS})
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${VALFUZZ_LINK_OPTIONS})

if (VALFUZZ_BUILD_TESTS)
  message("Building tests")
  
  add_executable(${PROJECT_NAME}_test
    ${VALFUZZ_TEST_SOURCES})
  target_include_directories(${PROJECT_NAME}_test PRIVATE
    ${VALFUZZ_INCLUDES})
  target_compile_options(${PROJECT_NAME}_test PRIVATE
    ${VALFUZZ_COMPILE_OPTIONS})
  target_link_libraries(${PROJECT_NAME}_test
    PRIVATE
    ${PROJECT_NAME}
    ${VALFUZZ_LINK_OPTIONS})
endif()

if (VALFUZZ_BUILD_CLOCK_PRECISION)
  message("Building clock_precision")
  add_executable(clock_precision
    utils/clock_precision.cpp)
  target_compile_options(clock_precision PRIVATE
    ${VALFUZZ_COMPILE_OPTIONS})
endif()
