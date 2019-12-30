#include <string>
#include <iostream>
#include "uint_t.hpp"

template<typename T>
T fib(const size_t n) {
    T a = 0, b = 1, c;
    for (size_t i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main() {
    const std::string ans = "16838932431231352876383";
    exit(fib<stlb::multiprecision::uint_t<75>>(13452334).to_string() != ans);
}
