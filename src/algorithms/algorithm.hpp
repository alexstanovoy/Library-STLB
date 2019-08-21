#ifndef __STLB_ALGO
#define __STLB_ALGO 1

#include <vector>
#include <iterator>
#include <algorithm>


namespace stlb
{

    template<class ForwardIt, class Getter>
    void radix_sort(const ForwardIt first, const ForwardIt last, const Getter& get_key) {
        using iter_type = typename std::iterator_traits<ForwardIt>::value_type;
        using sort_type = decltype(get_key(iter_type()));

        const sort_type BYTE = 0xFF;
        std::vector<size_t> n(0x100);
        std::vector<size_t> arr_ptr(0x100);
        std::vector<iter_type> arr(std::distance(first, last));

        for (sort_type bit_num = 0; bit_num < sort_type(8*sizeof(sort_type)); bit_num += 8) {
            n.assign(0x100, 0);
            std::for_each(first, last, [&get_key, &n, &BYTE, &bit_num](const iter_type& x){ 
                n[(get_key(x) & (BYTE << bit_num)) >> bit_num]++;
            });

            arr_ptr[0] = 0;
            for (size_t i = 1; i < 0x100; ++i) {
                arr_ptr[i] = arr_ptr[i - 1] + n[i - 1];
            }

            std::for_each(first, last, [&get_key, &arr, &arr_ptr, &BYTE, &bit_num](const iter_type& x){ 
                arr[arr_ptr[(get_key(x) & (BYTE << bit_num)) >> bit_num]++] = x;
            });

            std::copy(arr.begin(), arr.end(), first);
        }
    }


    template<class ForwardIt>
    void radix_sort(const ForwardIt first, const ForwardIt last) {
        using iter_type = typename std::iterator_traits<ForwardIt>::value_type;
        radix_sort(first, last, [](const iter_type& x){ return x; });
    }


    template<class ForwardIt, class Getter>
    auto radix_nth_element(const ForwardIt first, const ForwardIt last, size_t k, const Getter& get_key) {
        using iter_type = typename std::iterator_traits<ForwardIt>::value_type;
        using sort_type = decltype(get_key(iter_type()));

        const sort_type BYTE = 0xFF;
        sort_type legal_byte = 0;
        std::vector<size_t> n(0x100);

        for (sort_type bit_num = sort_type(8*sizeof(sort_type)-8);; bit_num -= 8) {
            sort_type cur_byte = (BYTE << bit_num);
            sort_type cur_st = (~sort_type(0)) - (sort_type(1) << (bit_num + 8)) + sort_type(1);
            n.assign(n.size(), 0);

            std::for_each(first, last, [&n, &get_key, cur_st, cur_byte, legal_byte, bit_num](const iter_type& x){
                sort_type tmp = get_key(x);
                if (((tmp & cur_st) ^ legal_byte) == 0) {
                    n[(tmp & cur_byte) >> bit_num]++;
                }
            });

            if (n[0] <= k) {
                for (size_t i = 1; i < 0x100; ++i) {
                    if ((n[i] += n[i - 1]) > k) {
                        legal_byte ^= (i << bit_num);
                        k -= n[i - 1];
                        break;
                    }
                }
            }

            if (bit_num == 0) {
                break;
            }
        }

        return legal_byte;
    }


    template<class ForwardIt>
    auto radix_nth_element(const ForwardIt first, const ForwardIt last, const size_t k) {
        using iter_type = typename std::iterator_traits<ForwardIt>::value_type;
        return radix_nth_element(first, last, k, [](const iter_type& x){ return x; });
    }


    template<class RandomIt>
    std::vector<size_t> prefix_function(const RandomIt first, const RandomIt last) {
        size_t len = std::distance(first, last);
        std::vector<size_t> ret(len, 0);
        for (size_t i = 1; i < len; ++i) {
            size_t j = ret[i - 1];
            while (j > 0 && *(first + i) != *(first + j)) {
                j = ret[j - 1];
            }
            if (*(first + i) == *(first + j)) {
                ++j;
            }
            ret[i] = j;
        }
        return ret;
    }


    template<class RandomIt>
    std::vector<size_t> z_function(const RandomIt first, const RandomIt last) {
        size_t len = std::distance(first, last);
        std::vector<size_t> ret(len, 0);
        for (size_t i = 1, l = 0, r = 0; i < len; ++i) {
            if (i <= r) {
                ret[i] = (ret[i - l] < r - i + 1 ? ret[i - l] : r - i + 1);
            }
            while (i + ret[i] < len && *(first + ret[i]) == *(first + i + ret[i])) {
                ++ret[i];
            }
            if (i + ret[i] - 1 > r) {
                l = i;
                r = i + ret[i] - 1;
            }
        }
        return ret;
    }


    template<class RandomIt>
    std::vector<std::pair<size_t, size_t>> manacher(const RandomIt first, const RandomIt last) {
        size_t len = std::distance(first, last);
        std::vector<std::pair<size_t, size_t>> ret(len, { 0, 0 });
        for (int64_t i = 0, l1 = 0, r1 = -1, l2 = 0, r2 = -1; i < len; ++i) {
            int64_t k1 = (i > r1 ? 1 : std::min(int64_t(ret[l1 + r1 - i].first), r1 - i + 1));
            int64_t k2 = (i > r2 ? 0 : std::min(int64_t(ret[l2 + r2 - i + 1].second), r2 - i + 1));
            while (i >= k1 && i + k1 < len && *(first + i - k1) == *(first + i + k1)) {
                ++k1;
            }
            while (i >= k2 + 1 && i + k2 < len && *(first + i - k2 - 1) == *(first + i + k2)) {
                ++k2;
            }
            ret[i].first = k1--;
            ret[i].second = k2--;
            if (i + k1 > r1) {
                l1 = i - k1;
                r1 = i + k1;
            }
            if (i + k2 > r2) {
                l2 = i - k2 - 1;
                r2 = i + k2;
            }
        }
        return ret;
    }

}

#endif //  __STLB_ALGO
