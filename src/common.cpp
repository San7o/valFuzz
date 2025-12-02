// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/common.hpp>

namespace valfuzz
{

std::mutex &get_stream_mutex()
{
#if __cplusplus >= 202002L // C++20
  constinit
#endif
    static std::mutex stream_mutex;
  return stream_mutex;
}

std::atomic<bool> &get_verbose()
{
#if __cplusplus >= 202002L // C++20
  constinit
#endif
    static std::atomic<bool>
      verbose = false;
  return verbose;
}

std::atomic<long unsigned int> &get_max_num_threads()
{
#if __cplusplus >= 202002L // C++20
  constinit
#endif
    static std::atomic<long unsigned int>
      max_num_threads = 4;
  return max_num_threads;
}

std::mutex &get_tests_mutex()
{
#if __cplusplus >= 202002L // C++20
  constinit
#endif
    static std::mutex tests_mutex;
  return tests_mutex;
}

std::atomic<bool> &get_is_threaded()
{
#if __cplusplus >= 202002L // C++20
  constinit
#endif
    static std::atomic<bool>
      is_threaded = true;
  return is_threaded;
}

std::vector<std::thread> &get_thread_pool()
{
  static std::vector<std::thread> thread_pool;
  return thread_pool;
}

} // namespace valfuzz
