#include "uint_t.hpp"

int main() {
    stlb::multiprecision::uint_t<512> valueA = 53645645;
    stlb::multiprecision::uint_t<512> valueB = 53645645;
    stlb::multiprecision::uint_t<512> valueC = 1;
    assert((valueA /= valueB) == valueC);
}
