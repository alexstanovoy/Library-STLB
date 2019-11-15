#include <string>
#include <iostream>
#include "uint_t.hpp"

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
    const std::string ans = "1347615707148841369192204067953771473429643549238341113781505897799744508412240184909438753863984547433408572552053041297363539128565227038245446402778857";
    exit(fib<stlb::multiprecision::uint_t<512>>(134534).to_string() != ans);
}
