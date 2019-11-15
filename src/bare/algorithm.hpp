#pragma once
#include <vector>
#include <climits>
#include <iterator>
#include <algorithm>


namespace stlb
{

    namespace algorithm
    {

        template<class ForwardIt, class Getter>
        void radix_sort(const ForwardIt first, const ForwardIt last, const Getter& get_key) {
            using iter_type = typename std::iterator_traits<ForwardIt>::value_type;
            using sort_type = decltype(get_key(iter_type()));

            const sort_type BYTE = 0xFF;
            const sort_type bytes_limit = sizeof(sort_type);
            std::vector<size_t> n(0x100);
            std::vector<size_t> arr_ptr(0x100);
            std::vector<iter_type> arr(std::distance(first, last));

            const auto get_byte = [&](const iter_type& x, const sort_type& bit_num) noexcept {
                return (get_key(x) & (BYTE << bit_num)) >> bit_num;
            };

            for (sort_type bit_num = 0; bit_num < sort_type(CHAR_BIT * bytes_limit); bit_num += CHAR_BIT) {
                std::fill(n.begin(), n.end(), 0);
                std::for_each(first, last, [&](const iter_type& x) { 
                    n[get_byte(x, bit_num)]++;
                });

                arr_ptr[0] = 0;
                for (size_t i = 1; i < 0x100; ++i) {
                    arr_ptr[i] = arr_ptr[i - 1] + n[i - 1];
                }

                std::for_each(first, last, [&](const iter_type& x) { 
                    arr[arr_ptr[get_byte(x, bit_num)]++] = x;
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
            const sort_type bytes_limit = sizeof(sort_type);
            sort_type legal_byte = 0;
            std::vector<size_t> n(0x100);

            for (sort_type bit_num = static_cast<sort_type>(CHAR_BIT * bytes_limit - CHAR_BIT);; bit_num -= CHAR_BIT) {
                sort_type cur_byte = (BYTE << bit_num);
                sort_type cur_st = (~static_cast<sort_type>(0)) - (static_cast<sort_type>(1) << (bit_num + 8)) + static_cast<sort_type>(1);
                std::fill(n.begin(), n.end(), 0);

                std::for_each(first, last, [&](const iter_type& x){
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
            for (ssize_t i = 0, l1 = 0, r1 = -1, l2 = 0, r2 = -1; i < static_cast<ssize_t>(len); ++i) {
                ssize_t k1 = (i > r1 ? 1 : std::min(ssize_t(ret[l1 + r1 - i].first), r1 - i + 1));
                ssize_t k2 = (i > r2 ? 0 : std::min(ssize_t(ret[l2 + r2 - i + 1].second), r2 - i + 1));
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


        struct operand
        {

            int32_t operands_number;
            const std::string str;

            operand(const int32_t operands_number, const std::string& str)
                : operands_number(operands_number), str(str) {}

        };


        class __infix_to_prefix
        {

        private:

            std::string _expr;
            std::vector<operand> _prefix;
            std::vector<operand> _func;
            std::vector<size_t> _comma_counter;

            bool is_digit_(const char symb) noexcept {
                return '0' <= symb && symb <= '9';
            }

            bool is_operand_(const char symb) noexcept {
                return symb == '+' || symb == '-' || symb == '*' || symb == '/';
            }

            bool is_alpha_(const char symb) noexcept {
                return ('a' <= symb && symb <= 'z') || ('A' <= symb && symb <= 'Z') || symb == '_';
            }

            std::string get_num_(size_t& ind) noexcept {
                std::string str;
                while (ind < _expr.size() && is_digit_(_expr[ind])) {
                    str += _expr[ind++];
                }
                return str;
            }

            std::string get_name_(size_t& ind) noexcept {
                std::string str;
                while (ind < _expr.size() && (is_digit_(_expr[ind]) || is_alpha_(_expr[ind]))) {
                    str += _expr[ind++];
                }
                return str;
            }

            void pull_to_prefix_() noexcept {
                _prefix.push_back(_func.back());
                _func.pop_back();
            }

            int32_t get_priority_(const operand& op) const noexcept {
                if (op.operands_number == 1) {
                    if (op.str.front() == '+' || op.str.front() == '-') {
                        return 30;
                    }
                }
                else {
                    if (op.str.front() == '+' || op.str.front() == '-') {
                        return 10;
                    }
                    if (op.str.front() == '*' || op.str.front() == '/') {
                        return 20;
                    }
                }
                return 0;
            }

            bool is_open_bracket_() const noexcept {
                return _func.back().str.front() == '(';
            }

            bool cmp_priority_(
                const operand& lhs,
                const operand& rhs
            ) const noexcept {
                return get_priority_(lhs) <= get_priority_(rhs);
            }

        public:

            __infix_to_prefix(const std::string& expression) noexcept {
                for (char i : expression) {
                    if (i != ' ') {
                        _expr += i;
                    }
                }
                _comma_counter.push_back(0);
            };

            std::vector<operand> convert() noexcept {
                for (size_t ind = 0; ind < _expr.size();) {
                    if (is_digit_(_expr[ind])) {
                        _prefix.emplace_back(-1, get_num_(ind));
                    }
                    else if (is_alpha_(_expr[ind])) {
                        std::string str = get_name_(ind);
                        if (ind >= _expr.size() || _expr[ind] != '(') {
                            _prefix.emplace_back(-1, str);
                        }
                        else {
                            _func.emplace_back(0, str);
                            _comma_counter.push_back(0);
                        }
                    }
                    else if (_expr[ind] == ',') {
                        while (!is_open_bracket_()) {
                            pull_to_prefix_();
                        }
                        ++ind, ++_comma_counter.back();
                    }
                    else if (is_operand_(_expr[ind])) {
                        operand current(
                            ind == 0 || is_operand_(_expr[ind - 1]) ||
                            _expr[ind - 1] == ',' || _expr[ind - 1] == '(' ? 1 : 2,
                            std::string(1, _expr[ind])
                        );
                        while (!_func.empty() && !is_open_bracket_() && cmp_priority_(current, _func.back())) {
                            pull_to_prefix_();
                        }
                        _func.push_back(current);
                        ++ind;
                    }
                    else if (_expr[ind] == '(') {
                        _func.emplace_back(0, "(");
                        ++ind;
                    }
                    else if (_expr[ind] == ')') {
                        while (!_func.empty() && !is_open_bracket_()) {
                            pull_to_prefix_();
                        }
                        _func.pop_back();
                        if (!_func.empty() && !is_open_bracket_() && _func.back().operands_number == 0) {
                            pull_to_prefix_();
                            if (_expr[ind - 1] != '(') {
                                _prefix.back().operands_number = _comma_counter.back() + 1;
                            }
                            _comma_counter.pop_back();
                        }
                        ++ind;
                    }
                }
                while (!_func.empty()) {
                    pull_to_prefix_();
                }
                return _prefix;
            }

        };


        auto infix_to_prefix(const std::string& str) noexcept {
            return __infix_to_prefix(str).convert();
        }

    }

}
