#include <vector>
#include <string>
#include <cassert>
#include "uint_t.hpp"

template<typename T>
T cnk(const size_t n, const size_t k) {
    std::vector<std::vector<T>> C(5001, std::vector<T>(5001));
    for (size_t i = 0; i <= 5000; ++i) {
        C[i][0] = C[i][i] = 1;
        for (size_t j = 1; j < i; ++j) {
            C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
        }
    }
    return C[n][k];
}

int main() {
    const std::string ans = "94630441838050288636931167897268592700261018000093644112499592955158105342540289857626";
    assert(cnk<stlb::multiprecision::uint_t<287>>(3965, 1403).to_string() == ans);
}
