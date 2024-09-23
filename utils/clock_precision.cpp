#include <chrono>
#include <iostream>

int main()
{
    std::cout << (double) std::chrono::high_resolution_clock::period::num
                 / std::chrono::high_resolution_clock::period::den;
    return 0;
}
