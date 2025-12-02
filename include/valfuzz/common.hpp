// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

namespace valfuzz
{

typedef std::function<void(std::string)> test_function;
typedef std::pair<std::string, test_function> test_pair;

std::mutex &get_stream_mutex();
std::atomic<bool> &get_verbose();
std::atomic<long unsigned int> &get_max_num_threads();
std::mutex &get_tests_mutex();
std::atomic<bool> &get_is_threaded();
std::vector<std::thread> &get_thread_pool();

} // namespace valfuzz
