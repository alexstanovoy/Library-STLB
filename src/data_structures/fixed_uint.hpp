#pragma once
#include <string>
#include <climits>
#include <iostream>
#include <algorithm>
#include "bitset.hpp"


namespace stlb
{

    template<const size_t _width>
    class fixed_uint
    {

    public:

        stlb::bitset<_width> _data;

        static constexpr const fixed_uint<_width> base_() noexcept {
            return fixed_uint<_width>(10000000000000000000ull);
        }

        static constexpr const fixed_uint<_width> base_f_(size_t pow) noexcept {
            unsigned long long ret = 1;
            do {
                ret *= 10;
            } while (--pow > 0);
            return ret;
        }

        static constexpr size_t base_size_() noexcept {
            return 19;
        }

    public:

        constexpr fixed_uint() noexcept = default;

        constexpr fixed_uint(const unsigned long long val) noexcept : _data(val) {}

        constexpr fixed_uint(const stlb::bitset<_width>& bit) noexcept : _data(bit) {}

        constexpr fixed_uint(const std::string& str) noexcept {
            for (size_t i = 0, len = str.size(); i < len; i += base_size_()) {
                *this = *this * (len - i < base_size_() ? base_f_(len - i) : base_()) +
                        fixed_uint<_width>(std::stoull(str.substr(i, base_size_())));
            }
        }

        ~fixed_uint() = default;

        explicit operator unsigned long long () const noexcept {
            return _data.to_ullong();
        }

        fixed_uint<_width>& operator<<= (const size_t pos) noexcept {
            _data <<= pos;
            return *this;
        }

        fixed_uint<_width>& operator>>= (const size_t pos) noexcept {
            _data >>= pos;
            return *this;
        }

        fixed_uint<_width> operator<< (const size_t pos) const noexcept {
            return fixed_uint<_width>(*this) <<= pos;
        }

        fixed_uint<_width> operator>> (const size_t pos) const noexcept {
            return fixed_uint<_width>(*this) >>= pos;
        }

        fixed_uint<_width> operator~ () const noexcept {
            return fixed_uint<_width>(~_data);
        }

        fixed_uint<_width>& operator^= (const fixed_uint<_width>& rhs) noexcept {
            _data ^= rhs._data;
            return *this;
        }

        fixed_uint<_width>& operator|= (const fixed_uint<_width>& rhs) noexcept {
            _data |= rhs._data;
            return *this;
        }

        fixed_uint<_width>& operator&= (const fixed_uint<_width>& rhs) noexcept {
            _data &= rhs._data;
            return *this;
        }

        friend fixed_uint<_width> operator^ (fixed_uint<_width> lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs ^= rhs;
        }

        friend fixed_uint<_width> operator| (fixed_uint<_width> lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs |= rhs;
        }

        friend fixed_uint<_width> operator& (fixed_uint<_width> lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs &= rhs;
        }

        friend bool operator== (const fixed_uint<_width>& lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs._data == rhs._data;
        }

        friend bool operator!= (const fixed_uint<_width>& lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs._data != rhs._data;
        }

        friend bool operator< (const fixed_uint<_width>& lhs, const fixed_uint<_width>& rhs) noexcept {
            const size_t diff = lhs._data.last_diff(rhs._data);
            return diff != _width && lhs._data.test(diff) < rhs._data.test(diff);
        }

        friend bool operator<= (const fixed_uint<_width>& lhs, const fixed_uint<_width>& rhs) noexcept {
            const size_t diff = lhs._data.last_diff(rhs._data);
            return diff == _width || lhs._data.test(diff) < rhs._data.test(diff);
        }

        friend bool operator> (const fixed_uint<_width>& lhs, const fixed_uint<_width>& rhs) noexcept {
            const size_t diff = lhs._data.last_diff(rhs._data);
            return diff != _width && lhs._data.test(diff) > rhs._data.test(diff);
        }

        friend bool operator>= (const fixed_uint<_width>& lhs, const fixed_uint<_width>& rhs) noexcept {
            const size_t diff = lhs._data.last_diff(rhs._data);
            return diff == _width || lhs._data.test(diff) > rhs._data.test(diff);
        }

        fixed_uint<_width>& operator++ () noexcept {
            for (auto& i : _data) {
                if (++i != 0) {
                    break;
                }
            }
            return *this;
        }

        fixed_uint<_width> operator++ (int) noexcept {
            const fixed_uint<_width> ret(*this);
            ++*this;
            return ret;
        }

        fixed_uint<_width>& operator-- () noexcept {
            for (auto& i : _data) {
                if (i-- != 0) {
                    break;
                }
            }
            return *this;
        }

        fixed_uint<_width> operator-- (int) noexcept {
            const fixed_uint<_width> ret(*this);
            --*this;
            return ret;
        }

        fixed_uint<_width> operator+ () const noexcept {
            return *this;
        }

        fixed_uint<_width> operator- () const noexcept {
            return ++(~*this);
        }

        fixed_uint<_width>& operator+= (const fixed_uint<_width>& rhs) noexcept {
            using word_t = typename std::iterator_traits<typename bitset<_width>::iterator>::value_type;
            word_t carry = 0;
            for (auto it1 = this->_data.begin(), it2 = rhs._data.begin(); it1 != this->_data.end(); ++it1, ++it2) {
                const word_t sum = *it1 + *it2;
                *it1 = sum + carry;
                carry = sum < *it2 || sum + carry < sum;
            }
            _data.normalize();
            return *this;
        }

        fixed_uint<_width>& operator-= (const fixed_uint<_width>& rhs) noexcept {
            return *this += -rhs;
        }

        fixed_uint<_width>& operator*= (const fixed_uint<_width>& rhs) noexcept {
            fixed_uint<_width> ret;
            for (size_t i = rhs._data.next_one(); i < _width; i = rhs._data.next_one(i + 1)) {
                ret += (*this << i);
            }
            return *this = ret;
        }

        fixed_uint<_width> operator/= (const fixed_uint<_width>& rhs) noexcept {
            if (*this < rhs){
                return fixed_uint<_width>();
            }
            if (*this == rhs) {
                return fixed_uint<_width>(1);
            }
            fixed_uint<_width> ret;
            for (ssize_t shift = _data.prev_one() - rhs._data.prev_one(); shift >= 0; --shift) {
                fixed_uint<_width> shifted(rhs << shift);
                if (*this >= shifted) {
                    *this -= shifted;
                    ret._data.flip(shift);
                }
            }
            return *this = ret;
        }

        friend fixed_uint<_width> operator+ (fixed_uint<_width> lhs, fixed_uint<_width> rhs) noexcept {
            return lhs += rhs;
        }

        friend fixed_uint<_width> operator- (fixed_uint<_width> lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs -= rhs;
        }

        friend fixed_uint<_width> operator* (fixed_uint<_width> lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs *= rhs;
        }

        friend fixed_uint<_width> operator/ (fixed_uint<_width> lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs /= rhs;
        }

        fixed_uint<_width> operator%= (const fixed_uint<_width>& rhs) noexcept {
            return *this -= rhs * (*this / rhs);
        }

        friend fixed_uint<_width> operator% (fixed_uint<_width> lhs, const fixed_uint<_width>& rhs) noexcept {
            return lhs %= rhs;
        }

        friend std::ostream& operator<< (std::ostream& os, const fixed_uint<_width>& val) {
            os << val.to_string();
            return os;
        }

        friend std::istream& operator>> (std::istream& is, fixed_uint<_width>& val) {
            std::string str;
            is >> str;
            val(str);
            return is;
        }

        std::string to_string() const noexcept {
            if (_width <= CHAR_BIT * sizeof(unsigned long long)) {
                return std::to_string(_data.to_ullong());
            }
            if (_data.none()) {
                return "0";
            }
            std::string ret;
            ret.reserve((_width + 2) / 3);
            for (fixed_uint<_width> next(*this / base_()), tmp(*this);; next = tmp / base_()) {
                const std::string str(std::to_string(static_cast<unsigned long long>(tmp - base_() * next)));
                std::for_each(str.rbegin(), str.rend(), [&ret](const char x) {
                    ret += x;
                });
                if (next._data.none()) {
                    break;
                }
                for (size_t i = str.size(); i < base_size_(); ++i) {
                    ret += '0';
                }
                tmp = next;
            }
            std::reverse(ret.begin(), ret.end());
            return ret;
        }

    };

}
