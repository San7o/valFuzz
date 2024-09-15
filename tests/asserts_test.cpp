#include "test/test.hpp"

TEST(assertion)
{
    ASSERT(1 == 1);
    ASSERT_EQ(1, 1);
    ASSERT_EQ(1, 2);
}
