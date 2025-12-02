// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>

int sum_int(int a, int b)
{
  return a + b;
}

FUZZME(naive_fuzzing, "Naive fuzzing")
{
  int a = valfuzz::get_random<int>();
  int b = valfuzz::get_random<int>();
  int ret = sum_int(a, b);
  ASSERT_EQ(ret, a + b);
}

int foo_bar(int a, int b)
{
  if (a == 0)
  {
    return 0;
  }
  if (b == 0)
  {
    return 0;
  }
  return a / b;
}

FUZZME(divide_fuzzing, "Divide fuzzing")
{
  int a = valfuzz::get_random<int>();
  int b = valfuzz::get_random<int>();
  int ret = foo_bar(a, b);
  if (b != 0)
  {
    ASSERT_EQ(ret, a / b);
  }
  else
  {
    ASSERT_EQ(ret, 0);
  }
}
