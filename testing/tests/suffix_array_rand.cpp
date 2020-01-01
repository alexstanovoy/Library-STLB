#include <vector>
#include <random>
#include <cassert>
#include <algorithm>
#include <functional>
#include "algorithm.hpp"

std::mt19937 rnd(6478482);

/* https://cp-algorithms.com/string/suffix-array.html */
std::vector<int> sort_cyclic_shifts(std::string const& s) {
    int n = s.size();
    const int alphabet = 256;
    std::vector<int> p(n), c(n), cnt(std::max(alphabet, n), 0);
    for (int i = 0; i < n; i++)
        cnt[s[i]]++;
    for (int i = 1; i < alphabet; i++)
        cnt[i] += cnt[i-1];
    for (int i = 0; i < n; i++)
        p[--cnt[s[i]]] = i;
    c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
        if (s[p[i]] != s[p[i-1]])
            classes++;
        c[p[i]] = classes - 1;
    }
    std::vector<int> pn(n), cn(n);
    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0)
                pn[i] += n;
        }
        std::fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int i = 0; i < n; i++)
            cnt[c[pn[i]]]++;
        for (int i = 1; i < classes; i++)
            cnt[i] += cnt[i-1];
        for (int i = n-1; i >= 0; i--)
            p[--cnt[c[pn[i]]]] = pn[i];
        cn[p[0]] = 0;
        classes = 1;
        for (int i = 1; i < n; i++) {
            std::pair<int, int> cur = {c[p[i]], c[(p[i] + (1 << h)) % n]};
            std::pair<int, int> prev = {c[p[i-1]], c[(p[i-1] + (1 << h)) % n]};
            if (cur != prev)
                ++classes;
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
    }
    return p;
}

std::vector<int> suffix_array_construction(std::string s) {
    s += '$';
    std::vector<int> sorted_shifts = sort_cyclic_shifts(s);
    sorted_shifts.erase(sorted_shifts.begin());
    return sorted_shifts;
}

void test() {
    const size_t N = 10000;
    std::string s1(N, 0);
    for (char& i : s1) {
        i = rnd() % 26 + 'a';
    }
    std::string s2 = s1;
    auto ans1 = stlb::algorithm::suffix_array(s1.begin(), s1.end());
    auto ans2 = suffix_array_construction(s2);
    assert(ans1.size() == ans2.size());
    for (size_t i = 0; i < N; ++i) {
        assert(ans1[i] == (size_t)ans2[i]);
    }
}

int main() {
    for (int i = 0; i < 500; ++i) {
        test();
    }
}
