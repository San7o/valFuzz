#include "valfuzz/valfuzz.hpp"

#include <print>

namespace valfuzz
{
std::vector<std::pair<std::string, std::function<void(std::string)>>>
    registered_tests;
} // namespace valfuzz

int main(int argc, char **argv)
{
    std::print("Starting tests, running {} tests\n",
               valfuzz::registered_tests.size());

    for (auto &test : valfuzz::registered_tests)
    {
        std::get<1>(test)(std::get<0>(test));
    }
    // get_args(a_function);

    std::print("Tests finished\n");
    return 0;
}
