// SPDX-License-Identifier: MIT
// Author:  Giovanni Santini
// Mail:    giovanni.santini@proton.me
// Github:  @San7o

#include <valfuzz/valfuzz.hpp>

BEFORE()
{
  std::cout << "Before test" << std::endl;
}

TEST(a_test, "Another simple test")
{
  ASSERT_EQ(1, 1);
}

AFTER()
{
  std::cout << "After test" << std::endl;
}
