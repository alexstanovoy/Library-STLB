#include <string>
#include <iostream>
#include "fixed_uint.hpp"

template<typename T>
T fib(const size_t n) {
    T a = 0, b = 1, c;
    if (n == 0) {
        return a;
    }
    if (n == 1) {
        return b;
    }
    for (size_t i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    const std::string ans = "16838932431231352876383";
    exit(fib<stlb::fixed_uint<75>>(13452334).to_string() != ans);
}
