#include "uint_t.hpp"


int main() {
    stlb::multiprecision::uint_t<512> value = 0;
    assert(value.to_string() == "0");
}
