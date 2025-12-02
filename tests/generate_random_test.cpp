// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>

TEST(random_int, "Generate a random int")
{
  int generated = valfuzz::get_random<int>();
  if (valfuzz::get_verbose())
  {
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
    std::cout << "Generated int: " << generated << "\n";
  }
  ASSERT_NE(generated, 0);
}

TEST(random_float, "Generate a random float")
{
  float generated = valfuzz::get_random<float>();
  if (valfuzz::get_verbose())
  {
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
    std::cout << "Generated float: " << generated << "\n";
  }
  ASSERT_NE(generated, 0);
}

TEST(random_double, "Generate a random double")
{
  double generated = valfuzz::get_random<double>();
  if (valfuzz::get_verbose())
  {
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
    std::cout << "Generated double: " << generated << "\n";
  }
  ASSERT_NE(generated, 0);
}

TEST(random_char, "Generate a random char")
{
  char generated = valfuzz::get_random<char>();
  if (valfuzz::get_verbose())
  {
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
    std::cout << "Generated char: " << generated << "\n";
  }
  ASSERT_NE(generated, 0);
}

TEST(random_bool, "Generate a random bool")
{
  bool generated = valfuzz::get_random<bool>();
  if (valfuzz::get_verbose())
  {
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
    std::cout << "Generated bool: " << generated << "\n";
  }
}

TEST(random_string, "Generate a random string")
{
  std::string generated = valfuzz::get_random<std::string>();
  if (valfuzz::get_verbose())
  {
    std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
    std::cout << "Generated string: " << generated << "\n";
  }
}
