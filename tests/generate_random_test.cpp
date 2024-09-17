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

TEST(random_int, "Generate a random int")
{
    int generated = valfuzz::get_random<int>();
    if (valfuzz::get_verbose())
    {
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
        std::print("Generated int: {}\n", generated);
    }
    ASSERT_NE(generated, 0);
}

TEST(random_float, "Generate a random float")
{
    float generated = valfuzz::get_random<float>();
    if (valfuzz::get_verbose())
    {
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
        std::print("Generated float: {}\n", generated);
    }
    ASSERT_NE(generated, 0);
}

TEST(random_double, "Generate a random double")
{
    double generated = valfuzz::get_random<double>();
    if (valfuzz::get_verbose())
    {
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
        std::print("Generated double: {}\n", generated);
    }
    ASSERT_NE(generated, 0);
}

TEST(random_char, "Generate a random char")
{
    char generated = valfuzz::get_random<char>();
    if (valfuzz::get_verbose())
    {
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
        std::print("Generated char: {}\n", generated);
    }
    ASSERT_NE(generated, 0);
}

TEST(random_bool, "Generate a random bool")
{
    bool generated = valfuzz::get_random<bool>();
    if (valfuzz::get_verbose())
    {
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
        std::print("Generated bool: {}\n", generated);
    }
}

TEST(random_string, "Generate a random string")
{
    std::string generated = valfuzz::get_random<std::string>();
    if (valfuzz::get_verbose())
    {
        std::lock_guard<std::mutex> lock(valfuzz::get_stream_mutex());
        std::print("Generated string: {}\n", generated);
    }
}
