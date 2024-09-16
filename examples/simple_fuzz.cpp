#include "valfuzz/valfuzz.hpp"

FUZZME(a_function, int a, int b)
{
    [[maybe_unused]] auto c = a + b;
    return 0;
}
