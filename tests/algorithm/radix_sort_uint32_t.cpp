#include <random>
#include <time.h>
#include <vector>
#include <algorithm>
#include "algorithm.hpp"


int main() {
    const size_t n = 1000000;
    std::mt19937 rnd(56734);

    std::vector<uint32_t> vec1(n);
    std::generate(vec1.begin(), vec1.end(), rnd);
    std::vector<uint32_t> vec2 = vec1;

    std::sort(vec1.begin(), vec1.end());
    stlb::radix_sort(vec2.begin(), vec2.end());

    exit(vec1 != vec2);
}
