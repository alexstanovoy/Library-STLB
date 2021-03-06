#include <random>
#include <time.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include "algorithm.hpp"


int main() {
    const size_t n = 100000;
    std::mt19937_64 rnd(45456);

    std::vector<unsigned __int128> vec1(n);
    std::generate(vec1.begin(), vec1.end(), [&]() {
        return (static_cast<unsigned __int128>(rnd()) << 64) + static_cast<unsigned __int128>(rnd());
    });
    std::vector<unsigned __int128> vec2 = vec1;

    std::sort(vec1.begin(), vec1.end());
    stlb::algorithm::radix_sort(vec2.begin(), vec2.end());

    assert(vec1 == vec2);
}
