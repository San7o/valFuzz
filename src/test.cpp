#include "test/test.hpp"

#include <print>

std::vector<std::function<void()>> registered_tests;

int main(int argc, char **argv)
{
    std::print("Starting tests, running {} tests\n", registered_tests.size());

    for (auto &test : registered_tests)
    {
        test();
    }
    // get_args(a_function);

    std::print("Tests finished\n");
    return 0;
}
