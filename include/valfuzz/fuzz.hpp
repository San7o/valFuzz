/*
 * MIT License
 *
 * Copyright (c) 2024 Giovanni Santini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#pragma once

#include "valfuzz/common.hpp"

#include <atomic>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <tuple>

namespace valfuzz
{

/* Fuzzer */

#define MAX_RANDOM_STRING_LEN 1024

template <typename T> T get_random();

#define FUZZME(fun_name, pretty_name)                                          \
    void fun_name([[maybe_unused]] const std::string &test_name);              \
    static struct fun_name##_register                                          \
    {                                                                          \
        fun_name##_register()                                                  \
        {                                                                      \
            valfuzz::add_fuzz_test(pretty_name, fun_name);                     \
        }                                                                      \
    } fun_name##_register_instance;                                            \
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
