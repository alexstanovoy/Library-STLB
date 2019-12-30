#include <random>
#include <cassert>
#include "earley_parser.hpp"


std::mt19937 rnd(434645);

std::string gen_correct(const size_t& len) noexcept {
    if (len == 0) {
        return "";
    }
    else {
        const size_t delim = 2 * (rnd() % (len / 2)) + 1;
        const size_t len1 = delim - 1;
        const size_t len2 = len - delim - 1;
        return '(' + gen_correct(len1) + ')' + gen_correct(len2);
    }
}

std::string gen() noexcept {
    std::string ret = gen_correct(2 * (rnd() % 100));
    if (rnd() % 2 == 0 && ret.size() != 0) {
        const size_t ind = rnd() % ret.size();
        ret[ind] = (ret[ind] == '(' ? ')' : '(');
    }
    return ret;
}

bool check_correct(const std::string& s) noexcept {
    ssize_t balance = 0;
    for (auto i : s) {
        balance += (i == '(' ? 1 : -1);
        if (balance < 0) {
            return false;
        }
    }
    return balance == 0;
}

int main() {
    char S = 'X';
    std::vector<char> N = { 'X', 'E' };
    std::vector<char> sigma = { '(', ')' };
    std::vector<stlb::utility::rule> P = {
        { 'X', "E" },
        { 'E', "(E)E" },
        { 'E', "" }
    };
    const auto parser = stlb::utility::earley_parser(stlb::utility::grammar(S, N, sigma, P));
    for (size_t i = 0; i < 5000; ++i) {
        const std::string generated = gen();
        assert(parser.calculate(generated) == check_correct(generated));
    }
}
