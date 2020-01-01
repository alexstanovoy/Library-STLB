#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <complex>
#include <climits>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>


namespace stlb
{

    namespace algorithm
    {

        template<class ByteGetter>
        class __radix_sort
        {

        private:

            const ByteGetter& _byte_getter;
            const size_t _byte_size;
            std::vector<size_t> _count;
            std::vector<size_t> _offset;

        public:

            __radix_sort(
                const ByteGetter& byte_getter,
                const size_t& byte_size,
                const size_t& bit_size
            ) noexcept : _byte_getter(byte_getter), _byte_size(byte_size),
                         _count(bit_size), _offset(bit_size) {}

            ~__radix_sort() noexcept = default;

            template<
                class InputForwardIt,
                class OutputForwardIt
            > void sort(
                const InputForwardIt& first,
                const InputForwardIt& last,
                const OutputForwardIt& out
            ) noexcept {
                using type = typename std::iterator_traits<InputForwardIt>::value_type;
                std::vector<type> tmp(std::distance(first, last));
                for (size_t byte_index = 0; byte_index < _byte_size; ++byte_index) {
                    std::fill(_count.begin(), _count.end(), 0);
                    std::for_each(first, last, [&](const type& val) noexcept {
                        ++_count[_byte_getter(val, byte_index)];
                    });
                    _offset.front() = 0;
                    std::partial_sum(_count.begin(), _count.end() - 1, _offset.begin() + 1);
                    std::for_each(first, last, [&](const type& val) noexcept {
                        tmp[_offset[_byte_getter(val, byte_index)]++] = val;
                    });
                    std::copy(tmp.begin(), tmp.end(), out);
                }
            }

        };

        template<class InputForwardIt>
        void radix_sort(
            const InputForwardIt& first,
            const InputForwardIt& last
        ) noexcept {
            using type = typename std::iterator_traits<InputForwardIt>::value_type;
            const auto& byte_getter = [](const type& val, const size_t& ind) noexcept {
                return (val >> (8 * ind)) & 0xFF;
            };
            __radix_sort<decltype(byte_getter)>(
                byte_getter, sizeof(type), 0x100
            ).sort(first, last, first);
        }

        template<
            class InputForwardIt,
            class OutputForwardIt
        > void radix_sort(
            const InputForwardIt& first,
            const InputForwardIt& last,
            const OutputForwardIt& out
        ) noexcept {
            using type = typename std::iterator_traits<InputForwardIt>::value_type;
            const auto& byte_getter = [](const type& val, const size_t& ind) noexcept {
                return (val >> (8 * ind)) & 0xFF;
            };
            __radix_sort<decltype(byte_getter)>(
                byte_getter, sizeof(type), 0x100
            ).sort(first, last, out);
        }

        template<
            class InputForwardIt,
            class ByteGetter
        > void radix_sort(
            const InputForwardIt& first,
            const InputForwardIt& last,
            const ByteGetter& byte_getter,
            const size_t& byte_size,
            const size_t& bit_size
        ) noexcept {
            __radix_sort<ByteGetter>(
                byte_getter, byte_size, bit_size
            ).sort(first, last, first);
        }

        template<
            class InputForwardIt,
            class OutputForwardIt,
            class ByteGetter
        > void radix_sort(
            const InputForwardIt& first,
            const InputForwardIt& last,
            const OutputForwardIt& out,
            const ByteGetter& byte_getter,
            const size_t& byte_size,
            const size_t& bit_size
        ) noexcept {
            __radix_sort<ByteGetter>(
                byte_getter, byte_size, bit_size
            ).sort(first, last, out);
        }

        template<class EqualTo>
        class __prefix_function
        {

        private:

            const EqualTo& _equal;

        public:

            __prefix_function(
                const EqualTo& equal
            ) noexcept : _equal(equal) {}

            ~__prefix_function() noexcept = default;

            template<class RandomIt>
            std::vector<size_t> calculate(
                const RandomIt& first,
                const RandomIt& last
            ) const noexcept {
                const size_t len = std::distance(first, last);
                std::vector<size_t> res(len);
                res.front() = 0;
                for (size_t ind = 1; ind < len; ++ind) {
                    size_t cur = res[ind - 1];
                    while (cur > 0 && !_equal(first[ind], first[cur])) {
                        cur = res[cur - 1];
                    }
                    if (_equal(first[ind], first[cur])) {
                        ++cur;
                    }
                    res[ind] = cur;
                }
                return res;
            }

        };

        template<class RandomIt>
        std::vector<size_t> prefix_function(
            const RandomIt& first,
            const RandomIt& last
        ) noexcept {
            using type = typename std::iterator_traits<RandomIt>::value_type;
            return __prefix_function<decltype(std::equal_to<type>())>(
                std::equal_to<type>()
            ).calculate(first, last);
        }

        template<
            class RandomIt,
            class EqualTo
        > std::vector<size_t> prefix_function(
            const RandomIt& first,
            const RandomIt& last,
            const EqualTo& equal
        ) noexcept {
            return __prefix_function<EqualTo>(equal).calculate(first, last);
        }

        template<class EqualTo>
        class __z_function
        {

        private:

            const EqualTo& _equal;

        public:

            __z_function(
                const EqualTo& equal
            ) noexcept : _equal(equal) {}

            ~__z_function() noexcept = default;

            template<class RandomIt>
            std::vector<size_t> calculate(
                const RandomIt& first,
                const RandomIt& last
            ) const noexcept {
                const size_t len = std::distance(first, last);
                std::vector<size_t> ret(len, 0);
                for (size_t i = 1, l = 0, r = 0; i < len; ++i) {
                    if (i <= r) {
                        ret[i] = (ret[i - l] < r - i + 1 ? ret[i - l] : r - i + 1);
                    }
                    while (i + ret[i] < len && _equal(first[ret[i]], first[i + ret[i]])) {
                        ++ret[i];
                    }
                    if (i + ret[i] - 1 > r) {
                        l = i;
                        r = i + ret[i] - 1;
                    }
                }
                return ret;
            }

        };

        template<class RandomIt>
        std::vector<size_t> z_function(
            const RandomIt& first,
            const RandomIt& last
        ) noexcept {
            using type = typename std::iterator_traits<RandomIt>::value_type;
            return __z_function<decltype(std::equal_to<type>())>(
                std::equal_to<type>()
            ).calculate(first, last);
        }

        template<
            class RandomIt,
            class EqualTo
        > std::vector<size_t> z_function(
            const RandomIt& first,
            const RandomIt& last,
            const EqualTo& equal
        ) noexcept {
            return __z_function<EqualTo>(equal).calculate(first, last);
        }

        template<class EqualTo>
        class __manacher
        {

        private:

            const EqualTo& _equal;

            template<class RandomIt>
            void odd_(
                const RandomIt& first,
                const ssize_t& len,
                std::vector<std::pair<size_t, size_t>>& res
            ) const noexcept {
                for (ssize_t i = 0, l = 0, r = -1; i < len; ++i) {
                    ssize_t k = (i > r ? 1 : std::min(ssize_t(res[l + r - i].first), r - i + 1));
                    while (i >= k && i + k < len && _equal(first[i - k], first[i + k])) {
                        ++k;
                    }
                    res[i].first = k--;
                    if (i + k > r) {
                        l = i - k;
                        r = i + k;
                    }
                }
            }

            template<class RandomIt>
            void even_(
                const RandomIt& first,
                const ssize_t& len,
                std::vector<std::pair<size_t, size_t>>& res
            ) const noexcept {
                for (ssize_t i = 0, l = 0, r = -1; i < len; ++i) {
                    ssize_t k = (i > r ? 0 : std::min(ssize_t(res[l + r - i + 1].second), r - i + 1));
                    while (i >= k + 1 && i + k < len && _equal(first[i - k - 1], first[i + k])) {
                        ++k;
                    }
                    res[i].second = k--;
                    if (i + k > r) {
                        l = i - k - 1;
                        r = i + k;
                    }
                }
            }

        public:

            __manacher(
                const EqualTo& equal
            ) noexcept : _equal(equal) {}

            ~__manacher() noexcept = default;

            template<class RandomIt>
            std::vector<std::pair<size_t, size_t>> calculate(
                const RandomIt& first,
                const RandomIt& last
            ) const noexcept {
                const ssize_t len = std::distance(first, last);
                std::vector<std::pair<size_t, size_t>> res(len, std::make_pair(0, 0));
                odd_(first, len, res);
                even_(first, len, res);
                return res;
            }

        };

        template<class RandomIt>
        std::vector<std::pair<size_t, size_t>> manacher(
            const RandomIt& first,
            const RandomIt& last
        ) noexcept {
            using type = typename std::iterator_traits<RandomIt>::value_type;
            return __manacher<decltype(std::equal_to<type>())>(
                std::equal_to<type>()
            ).calculate(first, last);
        }

        template<
            class RandomIt,
            class EqualTo
        > std::vector<std::pair<size_t, size_t>> manacher(
            const RandomIt& first,
            const RandomIt& last,
            const EqualTo& equal
        ) noexcept {
            return __manacher<EqualTo>(equal).calculate(first, last);
        }

        class __fast_fourier_transform
        {

        private:

            using complex_d = std::complex<double>;

            void reverse_(
                const size_t& len,
                std::vector<complex_d>& vec
            ) const noexcept {
                for (size_t i = 1, j = 0; i < len; ++i) {
                    size_t bit = len / 2;
                    for (; j & bit; bit /= 2) {
                        j ^= bit;
                    }
                    j ^= bit;
                    if (i < j) {
                        std::swap(vec[i], vec[j]);
                    }
                }
            }

            void calculate_(
                const size_t& len,
                std::vector<complex_d>& vec,
                const double& coef
            ) const noexcept {
                for (size_t m = 2; m <= len; m *= 2) {
                    const size_t m_2 = m / 2;
                    const double angle = coef / m;
                    const complex_d wlen(cos(angle), sin(angle));
                    for (size_t i = 0; i < len; i += m) {
                        complex_d w(1);
                        for (size_t j = 0; j < m_2; ++j) {
                            const complex_d u(vec[i + j]);
                            const complex_d v(vec[i + j + m_2] * w);
                            vec[i + j] = u + v;
                            vec[i + j + m_2] = u - v;
                            w *= wlen;
                        }
                    }
                }
            }

            void inverse_(
                const size_t& len,
                std::vector<complex_d>& vec
            ) const noexcept {
                for (complex_d& i : vec) {
                    i /= len;
                }
            }

            void fft_(
                std::vector<complex_d>& vec,
                const bool& invert
            ) const noexcept {
                const size_t len = vec.size();
                reverse_(len, vec);
                calculate_(len, vec, (invert ? -2 : 2) * M_PI);
                if (invert) {
                    inverse_(len, vec);
                }
            }

            void multiplicate_(
                const size_t& len,
                std::vector<complex_d>& vec1,
                std::vector<complex_d>& vec2
            ) const noexcept {
                fft_(vec1, false);
                fft_(vec2, false);
                for (size_t i = 0; i < len; ++i) {
                    vec1[i] *= vec2[i];
                }
                fft_(vec1, true);
            }

            template<class type>
            std::vector<type> finalize_(
                const size_t& len,
                std::vector<complex_d>& vec
            ) const noexcept {
                std::vector<type> ret(len);
                for (size_t i = 0; i < len; ++i) {
                    ret[i] = static_cast<type>(vec[i].real() + 0.5);
                }
                return ret;
            }

            size_t upper_two_degree_(
                const size_t& val
            ) const noexcept {
                size_t ret = 1;
                while (ret < val) {
                    ret *= 2;
                }
                return ret;
            }

        public:

            __fast_fourier_transform() noexcept = default;

            ~__fast_fourier_transform() noexcept = default;

            template<class RandomIt>
            auto calculate(
                const RandomIt& first1,
                const RandomIt& last1,
                const RandomIt& first2,
                const RandomIt& last2
            ) const noexcept {
                using type = typename std::iterator_traits<RandomIt>::value_type;
                const size_t len1 = std::distance(first1, last1);
                const size_t len2 = std::distance(first2, last2);
                const size_t len = upper_two_degree_(len1 + len2);
                std::vector<complex_d> vec1(len, 0);
                std::vector<complex_d> vec2(len, 0);
                std::copy(first1, last1, vec1.begin());
                std::copy(first2, last2, vec2.begin());
                multiplicate_(len, vec1, vec2);
                return finalize_<type>(len, vec1);
            }

        };

        template<class RandomIt>
        auto fast_fourier_transform(
            const RandomIt& first1,
            const RandomIt& last1,
            const RandomIt& first2,
            const RandomIt& last2
        ) noexcept {
            return __fast_fourier_transform().calculate(
                first1, last1,
                first2, last2
            );
        }

        class __suffix_array
        {

        private:

            using iter_t = typename std::vector<size_t>::iterator;

            const size_t _alphabet;
            std::vector<size_t> _count;

            bool leq_(
                const size_t& a1,
                const size_t& a2,
                const size_t& b1,
                const size_t& b2
            ) const noexcept {
                return a1 < b1 || (a1 == b1 && a2 <= b2);
            }

            bool leq_(
                const size_t& a1,
                const size_t& a2,
                const size_t& a3,
                const size_t& b1,
                const size_t& b2,
                const size_t& b3
            ) const noexcept {
                return a1 < b1 || (a1 == b1 && leq_(a2, a3, b2, b3));
            }

            void radix_pass_(
                const iter_t& first,
                const iter_t& last,
                const iter_t& out,
                const iter_t& key,
                const size_t& sigma
            ) noexcept {
                std::fill(_count.begin(), _count.begin() + sigma + 1, 0);
                std::for_each(first, last, [&](const size_t& val) noexcept {
                    ++_count[key[val]];
                });
                for (size_t i = 0, sum = 0; i <= sigma; i++) {
                    const size_t t = _count[i];
                    _count[i] = sum;
                    sum += t;
                }
                std::for_each(first, last, [&](const size_t& val) noexcept {
                    out[_count[key[val]]++] = val;
                });
            }

            void mod_sort_(
                const size_t& n02,
                const size_t& lim,
                const size_t& sigma,
                const iter_t& in,
                const iter_t& s12,
                const iter_t& SA12 
            ) noexcept {
                for (size_t i = 0, j = 0; i < lim; ++i) {
                    if (i % 3 != 0) {
                        s12[j++] = i;
                    }
                }
                radix_pass_(s12, s12 + n02, SA12, in + 2, sigma);
                radix_pass_(SA12, SA12 + n02, s12, in + 1, sigma);
                radix_pass_(s12, s12 + n02, SA12, in, sigma);
            }

            size_t rearrange_(
                const size_t& n0,
                const size_t& n02,
                const iter_t& in,
                const iter_t& s12,
                const iter_t& SA12
            ) const noexcept {
                size_t name = 0;
                size_t c0 = ~static_cast<size_t>(0);
                size_t c1 = ~static_cast<size_t>(0);
                size_t c2 = ~static_cast<size_t>(0);
                std::for_each(SA12, SA12 + n02, [&](const size_t& val) noexcept {
                    if (in[val] != c0 || in[val + 1] != c1 || in[val + 2] != c2) {
                        ++name;
                        c0 = in[val];
                        c1 = in[val + 1];
                        c2 = in[val + 2];
                    }
                    if (val % 3 == 1) {
                        s12[val / 3] = name;
                    }
                    else {
                        s12[val / 3 + n0] = name;
                    }
                });
                return name;
            }

            void finalize_(
                const size_t& n0,
                const size_t& n1,
                const size_t& n02,
                const size_t& len,
                const iter_t& in,
                const iter_t& s12,
                const iter_t& SA,
                const iter_t& SA0,
                const iter_t& SA12
            ) const noexcept {
                for (size_t p = 0, t = n0 - n1, k = 0; k < len; ++k) {
                    const size_t i = (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2);
                    const size_t j = SA0[p];
                    if (SA12[t] < n0 ?
                        leq_(in[i], s12[SA12[t] + n0], in[j], s12[j / 3]) :
                        leq_(in[i], in[i + 1], s12[SA12[t] - n0 + 1], in[j], in[j + 1], s12[j / 3 + n0])
                    ) {
                        SA[k] = i;
                        if (++t == n02) {
                            for (++k; p < n0; ++p, ++k) {
                                SA[k] = SA0[p];
                            }
                        }
                    }
                    else {
                        SA[k] = j;
                        if (++p == n0) {
                            for (++k; t < n02; ++t, ++k) {
                                SA[k] = (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2);
                            }
                        }
                    }
                }
            }

            void calculate_(
                const iter_t& in,
                const iter_t& SA,
                const size_t& len,
                const size_t& sigma
            ) noexcept {
                const size_t n0 = (len + 2) / 3;
                const size_t n1 = (len + 1) / 3;
                const size_t n2 = len / 3;
                const size_t n02 = n0 + n2;
                std::vector<size_t> s12(n02 + 3, 0);
                std::vector<size_t> SA12(n02 + 3, 0);
                std::vector<size_t> s0(n0, 0);
                std::vector<size_t> SA0(n0, 0);

                mod_sort_(
                    n02, len + (n0 - n1), sigma, in,
                    s12.begin(), SA12.begin()
                );
                const size_t name = rearrange_(n0, n02, in, s12.begin(), SA12.begin());

                if (name < n02) {
                    calculate_(s12.begin(), SA12.begin(), n02, name);
                    for (size_t i = 0; i < n02; ++i) {
                        s12[SA12[i]] = i + 1;
                    }
                }
                else {
                    for (size_t i = 0; i < n02; ++i) {
                        SA12[s12[i] - 1] = i;
                    }
                }
                for (size_t i = 0, j = 0; i < n02; ++i) {
                    if (SA12[i] < n0) {
                        s0[j++] = 3 * SA12[i];
                    }
                }
                radix_pass_(s0.begin(), s0.begin() + n0, SA0.begin(), in, sigma);

                finalize_(
                    n0, n1, n02, len, in, s12.begin(),
                    SA, SA0.begin(), SA12.begin()
                );
            }

        public:

            __suffix_array(
                const size_t& alphabet
            ) noexcept : _alphabet(alphabet) {}

            ~__suffix_array() noexcept = default;

            template<class RandomIt>
            std::vector<size_t> calculate(
                const RandomIt& first,
                const RandomIt& last
            ) noexcept {
                const size_t len = std::distance(first, last);
                std::vector<size_t> input(len + 3, 0);
                std::copy(first, last, input.begin());
                std::vector<size_t> output(len);
                _count.resize(std::max(_alphabet, len) + 1);
                calculate_(
                    input.begin(), output.begin(),
                    len, _alphabet
                );
                return output;
            }

        };

        template<class RandomIt>
        std::vector<size_t> suffix_array(
            const RandomIt& first,
            const RandomIt& last
        ) noexcept {
            return __suffix_array(0x100).calculate(first, last);
        }

        template<class RandomIt>
        std::vector<size_t> suffix_array(
            const RandomIt& first,
            const RandomIt& last,
            const size_t& alphabet
        ) noexcept {
            return __suffix_array(alphabet).calculate(first, last);
        }

        class __lcp_array
        {

        public:

            __lcp_array() noexcept = default;

            ~__lcp_array() noexcept = default;

            template<
                class StringRandomIt,
                class SuffixRandomIt
            > std::vector<size_t> calculate(
                const StringRandomIt& str_first,
                const StringRandomIt& str_last,
                const SuffixRandomIt& suf_first
            ) const noexcept {
                const size_t len = std::distance(str_first, str_last);
                std::vector<size_t> lcp(len);
                std::vector<size_t> rank(len);
                for (size_t i = 0; i < len; ++i) {
                    rank[suf_first[i]] = i;
                }
                for (size_t i = 0, k = 0; i < len; ++i, k != 0 ? --k : 0) {
                    if (rank[i] + 1 == len) {
                        k = 0;
                        continue;
                    }
                    const size_t& j = suf_first[rank[i] + 1];
                    while (i + k < len && j + k < len && str_first[i + k] == str_first[j + k]) {
                        ++k;
                    }
                    lcp[rank[i]] = k;

                }
                return lcp;
            }

        };

        template<class StringRandomIt, class SuffixRandomIt>
        std::vector<size_t> lcp_array(
            const StringRandomIt& str_first,
            const StringRandomIt& str_last,
            const SuffixRandomIt& suf_first
        ) noexcept {
            return __lcp_array().calculate(
                str_first, str_last, suf_first
            );
        }

        template<class StringRandomIt>
        std::vector<size_t> lcp_array(
            const StringRandomIt& str_first,
            const StringRandomIt& str_last
        ) noexcept {
            const std::vector<size_t> suf_array = suffix_array(str_first, str_last);
            return __lcp_array().calculate(
                str_first, str_last, suf_array.begin()
            );
        }

    }

}
