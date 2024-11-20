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

#include "valfuzz/benchmark.hpp"
#include "valfuzz/common.hpp"
#include "valfuzz/fuzz.hpp"
#include "valfuzz/test.hpp"
#include "valfuzz/reporter.hpp"

#include <atomic>
#include <deque>
#include <fstream>
#include <optional>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>

namespace valfuzz
{

std::atomic<bool> &get_header();
std::atomic<bool> &get_do_fuzzing();
std::optional<std::string> &get_test_one();
std::optional<std::string> &get_fuzz_one();

void set_multithreaded(bool is_threaded);
void set_max_num_threads(long unsigned int max_num_threads);
void set_verbose(bool verbose);
void set_header(bool header);
void set_do_fuzzing(bool do_fuzzing);
void set_test_one(const std::string &test_one);
void set_fuzz_one(const std::string &fuzz_one);

void parse_args(int argc, char *argv[]);
void print_header();

} // namespace valfuzz
