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
#include <valfuzz/benchmark.hpp>
#include <valfuzz/common.hpp>
#include <valfuzz/fuzz.hpp>
#include <valfuzz/reporter.hpp>
#include <valfuzz/test.hpp>

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

int main(int argc, char **argv);

} // namespace valfuzz
