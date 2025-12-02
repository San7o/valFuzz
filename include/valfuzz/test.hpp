// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <atomic>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <tuple>
#include <valfuzz/common.hpp>

/* Assertions */

#define ASSERT(cond)                                                           \
  if (!(cond))                                                                 \
  {                                                                            \
    valfuzz::set_has_failed_once(true);                                        \
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());             \
    std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";      \
    std::cerr << "Assertion failed: " << #cond << std::endl;                   \
  }

#define ASSERT_EQ(a, b)                                                        \
  if ((a) != (b))                                                              \
  {                                                                            \
    valfuzz::set_has_failed_once(true);                                        \
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());             \
    std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";      \
    std::cerr << "Assertion failed: " << #a << " != " << #b << std::endl;      \
  }

#define ASSERT_NE(a, b)                                                        \
  if ((a) == (b))                                                              \
  {                                                                            \
    valfuzz::set_has_failed_once(true);                                        \
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());             \
    std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";      \
    std::cerr << "Assertion failed: " << #a << " == " << #b << std::endl;      \
  }

#define ASSERT_LT(a, b)                                                        \
  if ((a) >= (b))                                                              \
  {                                                                            \
    valfuzz::set_has_failed_once(true);                                        \
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());             \
    std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";      \
    std::cerr << "Assertion failed: " << #a << " < " << #b << std::endl;       \
  }

#define ASSERT_LE(a, b)                                                        \
  if ((a) > (b))                                                               \
  {                                                                            \
    valfuzz::set_has_failed_once(true);                                        \
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());             \
    std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";      \
    std::cerr << "Assertion failed: " << #a << " <= " << #b << std::endl;      \
  }

#define ASSERT_GT(a, b)                                                        \
  if ((a) <= (b))                                                              \
  {                                                                            \
    valfuzz::set_has_failed_once(true);                                        \
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());             \
    std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";      \
    std::cerr << "Assertion failed: " << #a << " > " << #b << std::endl;       \
  }

#define ASSERT_GE(a, b)                                                        \
  if ((a) < (b))                                                               \
  {                                                                            \
    valfuzz::set_has_failed_once(true);                                        \
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());             \
    std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";      \
    std::cerr << "Assertion failed: " << #a << " >= " << #b << std::endl;      \
  }

#define ASSERT_THROW(expr, exception)                                          \
  {                                                                            \
    bool exception_thrown = false;                                             \
    try                                                                        \
    {                                                                          \
      expr;                                                                    \
    }                                                                          \
    catch (const exception &e)                                                 \
    {                                                                          \
      exception_thrown = true;                                                 \
    }                                                                          \
    if (!exception_thrown)                                                     \
    {                                                                          \
      valfuzz::set_has_failed_once(true);                                      \
      std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());           \
      std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";    \
      std::cerr << "Exception not thrown: " << #exception << std::endl;        \
    }                                                                          \
  }

#define ASSERT_NO_THROW(expr)                                                  \
  {                                                                            \
    try                                                                        \
    {                                                                          \
      expr;                                                                    \
    }                                                                          \
    catch (...)                                                                \
    {                                                                          \
      valfuzz::set_has_failed_once(true);                                      \
      std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());           \
      std::cerr << "test: " << test_name << ", line: " << __LINE__ << ", ";    \
      std::cerr << "Exception thrown" << std::endl;                            \
    }                                                                          \
  }

/* Tests */

namespace valfuzz
{

std::deque<valfuzz::test_pair> &get_tests();
std::mutex &get_test_mutex();
long unsigned int get_num_tests();
std::atomic<bool> &get_has_failed_once();

std::function<void()> &get_function_execute_before();
std::function<void()> &get_function_execute_after();

void set_function_execute_before(std::function<void()> f);
void set_function_execute_after(std::function<void()> f);
void set_has_failed_once(bool has_failed_once);

void add_test(const std::string &name, test_function test);

std::optional<valfuzz::test_pair> pop_test_or_null();
void run_one_test(const std::string &name);
void _run_tests();
void run_tests();

#define TEST(name, pretty_name)                                                \
  void name([[maybe_unused]] const std::string &test_name);                    \
  static struct name##_register                                                \
  {                                                                            \
    name##_register()                                                          \
    {                                                                          \
      valfuzz::add_test(pretty_name, name);                                    \
    }                                                                          \
  } name##_register_instance;                                                  \
  void name([[maybe_unused]] const std::string &test_name)

#define BEFORE()                                                               \
  void before();                                                               \
  static struct before##_register                                              \
  {                                                                            \
    before##_register()                                                        \
    {                                                                          \
      valfuzz::set_function_execute_before(before);                            \
    }                                                                          \
  } before##_register_instance;                                                \
  void before()

#define AFTER()                                                                \
  void after();                                                                \
  static struct after##_register                                               \
  {                                                                            \
    after##_register()                                                         \
    {                                                                          \
      valfuzz::set_function_execute_after(after);                              \
    }                                                                          \
  } after##_register_instance;                                                 \
  void after()

} // namespace valfuzz
