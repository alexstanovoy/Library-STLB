#include <random>
#include <cassert>
#include "earley_parser.hpp"
#include <iostream>

std::mt19937 rnd(48913443);
bool correct;

std::string number() {
    std::string ret(rnd() % 10 + 1, '0');
    ret[0] = rnd() % 9 + 1 + '0';
    for (size_t i = 1; i < ret.size(); ++i) {
        ret[i] = rnd() % 10 + '0';
    }
    return ret;
}

char operation() {
    switch (rnd() % 4) {
        case 0: {
            return '+';
        }
        case 1: {
            return '-';
        }
        case 2: {
            return '*';
        }
        case 3: {
            return '/';
        }
        default: {
            return '+';
        }
    }
}

std::string expr(const size_t& depth) {
    if (depth == 0 || rnd() % 5 == 0) {
        return number();
    }
    else if (rnd() % 2 == 0) {
        return '(' + expr(depth - 1) + ')';
    }
    else {
        return expr(depth - 1) + operation() + expr(depth - 1);
    }
}

std::string gen() {
    std::string ret = expr(rnd() % 20);
    if (rnd() % 3 == 0) {
        std::vector<size_t> pos;
        for (size_t i = 0; i < ret.size(); ++i) {
            if (ret[i] == '(' || ret[i] == ')') {
                pos.push_back(i);
            }
            else if (i >= 1 && i + 1 < ret.size() &&
                     '0' <= ret[i - 1] && ret[i] <= '9' &&
                     '0' <= ret[i + 1] && ret[i] <= '9') {
                pos.push_back(i);
            }
            else if (ret[i] == '+' || ret[i] == '-' ||
                     ret[i] == '*' || ret[i] == '/') {
                pos.push_back(i);
            }
        }
        if (!pos.empty()) {
            correct = false;
            const size_t i = pos[rnd() % pos.size()];
            if (ret[i] == '(') {
                ret[i] = ')';
            }
            else if (ret[i] == ')') {
                ret[i] = '(';
            }
            else if (i >= 1 && i + 1 < ret.size() &&
                     '0' <= ret[i - 1] && ret[i] <= '9' &&
                     '0' <= ret[i + 1] && ret[i] <= '9') {
                ret[i] = (rnd() % 2 == 0 ? '(' : ')');
            }
            else if (ret[i] == '+' || ret[i] == '-' ||
                     ret[i] == '*' || ret[i] == '/') {
                ret[i] = (rnd() % 2 == 0 ? '(' : ')');
            }
        }
        else {
            correct = true;
        }
    }
    else {
        correct = true;
    }
    return ret;
}

int main() {
    char S = 'X';
    std::vector<char> N = { 'X', 'D', 'N', 'O', 'E' };
    std::vector<char> sigma = { '(', ')', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
    std::vector<stlb::utility::rule> P = {
        { 'X', "E" },
        { 'D', "0" },
        { 'D', "1" },
        { 'D', "2" },
        { 'D', "3" },
        { 'D', "4" },
        { 'D', "5" },
        { 'D', "6" },
        { 'D', "7" },
        { 'D', "8" },
        { 'D', "9" },
        { 'N', "D" },
        { 'N', "DN" },
        { 'O', "+" },
        { 'O', "-" },
        { 'O', "*" },
        { 'O', "/" },
        { 'E', "N" },
        { 'E', "(E)" },
        { 'E', "EOE" }
    };
    const auto parser = stlb::utility::earley_parser(stlb::utility::grammar(S, N, sigma, P));
    for (size_t i = 0; i < 500; ++i) {
        const std::string s = gen();
        assert(parser.calculate(s) == correct);
    }
}
