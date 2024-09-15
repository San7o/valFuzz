#include "valfuzz/valfuzz.hpp"

TEST(assertion, "Simple Assertion")
{
    ASSERT(1 == 1);
    ASSERT_EQ(1, 1);
    ASSERT_EQ(1, 2);
}
