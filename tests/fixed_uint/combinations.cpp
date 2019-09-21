#include <vector>
#include <string>
#include "fixed_uint.hpp"

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
    const std::string ans1 = "6208056655368386051304263784475616298793715704911304862325449018690017571859172680298706964592212866846502195951000252629008608094041756296703667941558880";
    const std::string ans2 = "3498590400";
    const std::string ans3 = "94630441838050288636931167897268592700261018000093644112499592955158105342540289857626";
    exit(
        cnk<stlb::fixed_uint<512>>(4984, 3073).to_string() != ans1 ||
        cnk<stlb::fixed_uint<33>>(4123, 2453).to_string() != ans2 ||
        cnk<stlb::fixed_uint<287>>(3965, 1403).to_string() != ans3
    );
}