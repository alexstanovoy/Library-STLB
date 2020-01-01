#include <random>
#include <time.h>
#include <vector>
#include <cassert>
#include <algorithm>
#include "algorithm.hpp"


int main() {
    const size_t n = 100000;
    std::mt19937 rnd(56734);

    std::vector<uint32_t> vec1(n);
    std::generate(vec1.begin(), vec1.end(), rnd);
    std::vector<uint32_t> vec2 = vec1;

    std::sort(vec1.begin(), vec1.end());
    stlb::algorithm::radix_sort(vec2.begin(), vec2.end());

    assert(vec1 == vec2);
}
