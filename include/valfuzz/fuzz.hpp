// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <algorithm>
#include <atomic>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <random>
#include <string>
#include <thread>
#include <tuple>
#include <valfuzz/common.hpp>

namespace valfuzz
{

/* Fuzzer */

#define MAX_RANDOM_STRING_LEN 1024

template <typename T> T get_random();

#define FUZZME(fun_name, pretty_name)                                          \
  void fun_name([[maybe_unused]] const std::string &test_name);                \
  static struct fun_name##_register                                            \
  {                                                                            \
    fun_name##_register()                                                      \
    {                                                                          \
      valfuzz::add_fuzz_test(pretty_name, fun_name);                           \
    }                                                                          \
  } fun_name##_register_instance;                                              \
  void fun_name([[maybe_unused]] const std::string &test_name)

typedef std::function<void(std::string)> fuzz_function;
typedef std::pair<std::string, fuzz_function> fuzz_pair;

std::deque<fuzz_pair> &get_fuzzs();
long unsigned int get_num_fuzz_tests();
std::atomic<long unsigned int> &get_iterations();
std::mt19937 &get_random_engine();
std::uniform_real_distribution<> &get_uniform_distribution();

void increment_iterations();
std::optional<fuzz_pair> pop_fuzz_or_null();
void add_fuzz_test(const std::string &name, fuzz_function test);
void run_one_fuzz(const std::string &name);
void _run_fuzz_tests();
void run_fuzz_tests();

} // namespace valfuzz
