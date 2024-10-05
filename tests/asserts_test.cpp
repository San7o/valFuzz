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

#include "valfuzz/valfuzz.hpp"

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
