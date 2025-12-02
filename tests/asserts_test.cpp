// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>

TEST(simple_assertion, "Simple Assertion")
{
  ASSERT(1 == 1);
}

TEST(simple_assertion_eq, "Simple Assertion EQ")
{
  ASSERT_EQ(1, 1);
}

TEST(simple_assertion_ne, "Simple Assertion NE")
{
  ASSERT_NE(1, 2);
}

TEST(simple_assertion_lt, "Simple Assertion LT")
{
  ASSERT_LT(1, 2);
}

TEST(simple_assertion_le, "Simple Assertion LE")
{
  ASSERT_LE(1, 1);
}

TEST(simple_assertion_gt, "Simple Assertion GT")
{
  ASSERT_GT(2, 1);
}

TEST(simple_assertion_ge, "Simple Assertion GE")
{
  ASSERT_GE(1, 1);
}

void foo()
{
  throw std::runtime_error("Error");
}

void bar()
{
}

TEST(shoud_throw, "Should Throw")
{
  ASSERT_THROW(foo(), std::runtime_error);
}

TEST(shoud_not_throw, "Should Not Throw")
{
  ASSERT_NO_THROW(bar());
}
