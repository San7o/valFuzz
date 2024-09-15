#pragma once

#include <any>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <typeinfo>
#include <vector>

/* Assertions */

#define ASSERT(cond)                                                           \
    if (!(cond))                                                               \
    {                                                                          \
        std::cerr << "Assertion failed: " << #cond << std::endl;               \
    }

#define ASSERT_EQ(a, b)                                                        \
    if ((a) != (b))                                                            \
    {                                                                          \
        std::cerr << "Assertion failed: " << #a << " != " << #b << std::endl;  \
    }

#define ASSERT_NE(a, b)                                                        \
    if ((a) == (b))                                                            \
    {                                                                          \
        std::cerr << "Assertion failed: " << #a << " == " << #b << std::endl;  \
    }

#define ASSERT_LT(a, b)                                                        \
    if ((a) >= (b))                                                            \
    {                                                                          \
        std::cerr << "Assertion failed: " << #a << " < " << #b << std::endl;   \
    }

#define ASSERT_LE(a, b)                                                        \
    if ((a) > (b))                                                             \
    {                                                                          \
        std::cerr << "Assertion failed: " << #a << " <= " << #b << std::endl;  \
    }

#define ASSERT_GT(a, b)                                                        \
    if ((a) <= (b))                                                            \
    {                                                                          \
        std::cerr << "Assertion failed: " << #a << " > " << #b << std::endl;   \
    }

#define ASSERT_GE(a, b)                                                        \
    if ((a) < (b))                                                             \
    {                                                                          \
        std::cerr << "Assertion failed: " << #a << " >= " << #b << std::endl;  \
    }

/* Tests */

extern std::vector<std::function<void()>> registered_tests;

#define TEST(name)                                                             \
    void name();                                                               \
    static struct name##_register                                              \
    {                                                                          \
        name##_register()                                                      \
        {                                                                      \
            registered_tests.push_back(name);                                  \
        }                                                                      \
    } name##_register_instance;                                                \
    void name()

/* Fuzzer */

#define FUZZME(fun_name, ...)                                                  \
    template <typename... Args> int fun_name(Args... args)                     \
    {                                                                          \
        return 0;                                                              \
    }                                                                          \
    template <> int fun_name(__VA_ARGS__)

template <typename T> void process_type(std::vector<std::any> a_vec)
{
    T t = T();
    a_vec.push_back(t);
}

template <typename...> void process_types(std::vector<std::any> &)
{
}

template <typename T, typename... Rest>
void process_types(std::vector<std::any> &a_vec)
{
    process_type<T>(a_vec);
    process_types<Rest...>(a_vec);
}

template <typename... Args> void get_args(int (*f)(Args...))
{
    std::vector<std::any> a_vec;
    process_types<Args...>(a_vec);

    /* debug printing */
    for (const auto &t : a_vec)
    {
        std::cout << t.type().name() << " ";
    }
}
