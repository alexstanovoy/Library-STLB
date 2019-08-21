#ifndef __STLB_FIXED_INT
#define __STLB_FIXED_INT 1

#include <bitset>
#include <string>
#include <iostream>
#include <algorithm>


namespace stlb
{

    template<const size_t width>
    class fixed_uint
    {

    private:

        static constexpr const fixed_uint<width> _base = fixed_uint<width>(10000000000000000000ull);

        std::bitset<width> _data;

        size_t upper_one_() const noexcept {
            size_t l = _data._Find_first(), r = width, ret = l;
            while (l < r) {
                size_t mid = (l + r) / 2;
                if (_data._Find_next(mid) == width) {
                    r = mid;
                    ret = mid;
                }
                else {
                    l = mid + 1;
                }
            }
            return ret;
        }

    public:

        constexpr fixed_uint() noexcept = default;

        constexpr fixed_uint(const uint64_t val) noexcept : _data(val) {}

        constexpr fixed_uint(const std::bitset<width>& bit) noexcept : _data(bit) {}

        constexpr fixed_uint(const std::string& str) noexcept {
            for (size_t i = 0, len = str.size(); i < len; i += 19) {
                *this = *this * _base + fixed_uint<width>(std::stoull(str.substr(i, 19)));
            }
        }

        ~fixed_uint() = default;

        explicit operator uint64_t() const noexcept {
            uint64_t ret = 0;
            for (size_t i = 0; i < (width < 64 ? width : 64); ++i) {
                ret ^= (uint64_t(_data[i]) << i);
            }
            return ret;
        }

        fixed_uint<width> operator<< (const size_t pos) const noexcept {
            return fixed_uint<width>(_data << pos);
        }

        fixed_uint<width>& operator<<= (const size_t pos) noexcept {
            _data <<= pos;
            return *this;
        }

        fixed_uint<width> operator>> (const size_t pos) const noexcept {
            return fixed_uint<width>(_data << pos);
        }

        fixed_uint<width>& operator>>= (const size_t pos) noexcept {
            _data >>= pos;
            return *this;
        }

        fixed_uint<width> operator~ () const noexcept {
            return fixed_uint<width>(~_data);
        }

        friend fixed_uint<width> operator^ (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            return fixed_uint<width>(lhs._data ^ rhs._data);
        }

        friend fixed_uint<width> operator| (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            return fixed_uint<width>(lhs._data | rhs._data);
        }

        friend fixed_uint<width> operator& (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            return fixed_uint<width>(lhs._data & rhs._data);
        }

        fixed_uint<width>& operator^= (const fixed_uint<width>& rhs) noexcept {
            _data ^= rhs._data;
            return *this;
        }

        fixed_uint<width>& operator|= (const fixed_uint<width>& rhs) noexcept {
            _data |= rhs._data;
            return *this;
        }

        fixed_uint<width>& operator&= (const fixed_uint<width>& rhs) noexcept {
            _data &= rhs._data;
            return *this;
        }

        friend bool operator== (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            return lhs._data == rhs._data;
        }

        friend bool operator!= (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            return lhs._data != rhs._data;
        }

        friend bool operator< (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            const size_t ind = (lhs ^ rhs).upper_one_();
            return (ind != width && lhs._data[ind] == 0);
        }

        friend bool operator<= (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            const size_t ind = (lhs ^ rhs).upper_one_();
            return (ind == width || lhs._data[ind] == 0);
        }

        friend bool operator> (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            const size_t ind = (lhs ^ rhs).upper_one_();
            return (ind != width && lhs._data[ind] == 1);
        }

        friend bool operator>= (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            const size_t ind = (lhs ^ rhs).upper_one_();
            return (ind == width || lhs._data[ind] == 1);
        }

        fixed_uint<width>& operator++ () noexcept {
            for (size_t i = 0; i < width; i++) {
                if (_data[i]) {
                    _data[i] = 0;
                }
                else {
                    _data[i] = 1;
                    break;
                }
            }
            return *this;
        }

        fixed_uint<width> operator++ (int) noexcept {
            const fixed_uint<width> ret(*this);
            ++*this;
            return ret;
        }

        fixed_uint<width>& operator-- () noexcept {
            for (size_t i = 0; i < width; i++) {
                if (_data[i]) {
                    _data[i] = 0;
                    break;
                }
                else {
                    _data[i] = 1;
                }
            }
            return *this;
        }

        fixed_uint<width> operator-- (int) noexcept {
            const fixed_uint<width> ret(*this);
            --*this;
            return ret;
        }

        fixed_uint<width> operator+ () const noexcept {
            return *this;
        }

        fixed_uint<width> operator- () const noexcept {
            return ++(~*this);
        }

        friend fixed_uint<width> operator+ (fixed_uint<width> lhs, fixed_uint<width> rhs) noexcept {
            while (rhs._data.any()) {
                const fixed_uint<width> carry(lhs & rhs);
                lhs ^= rhs;
                rhs = (carry << 1);
            }
            return lhs;
        }

        friend fixed_uint<width> operator- (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            return lhs + (-rhs);
        }

        friend fixed_uint<width> operator* (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            fixed_uint<width> ret(0);
            for (size_t i = 0; i < width; ++i) {
                if (rhs._data[i]) {
                    ret += (lhs << i);
                }
            }
            return ret;
        }

        friend fixed_uint<width> operator/ (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            if (lhs < rhs){
                return fixed_uint<width>(0);
            }
            if (lhs == rhs) {
                return fixed_uint<width>(1);
            }
            fixed_uint<width> tmp(lhs), ret(0);
            for (size_t shift = tmp.upper_one_() - rhs.upper_one_();; --shift) {
                if (tmp >= (rhs << shift)) {
                    tmp -= (rhs << shift);
                    ret ^= (fixed_uint<width>(1) << shift);
                }
                if (shift == 0) {
                    break;
                }
            }
            return ret;
        }

        friend fixed_uint<width> operator% (const fixed_uint<width>& lhs, const fixed_uint<width>& rhs) noexcept {
            return lhs - rhs * (lhs / rhs);
        }

        friend std::ostream& operator<< (std::ostream &os, const fixed_uint<width>& val) {
            os << val.to_string();
            return os;
        }

        friend std::istream& operator>> (std::istream &is, fixed_uint<width>& val) {
            std::string str;
            is >> str;
            val(str);
            return is;
        }

        fixed_uint<width>& operator+= (fixed_uint<width> rhs) noexcept {
            while (rhs._data.any()) {
                const fixed_uint<width> carry(*this & rhs);
                *this ^= rhs;
                rhs = (carry << 1);
            }
            return *this;
        }

        fixed_uint<width>& operator-= (const fixed_uint<width>& rhs) noexcept {
            return *this += (-rhs);
        }

        fixed_uint<width>& operator*= (const fixed_uint<width>& rhs) noexcept {
            fixed_uint<width> ret(0);
            for (size_t i = 0; i < width; ++i) {
                if (rhs._data[i]) {
                    ret += (*this << i);
                }
            }
            return (*this = ret);
        }

        fixed_uint<width> operator/= (const fixed_uint<width>& rhs) noexcept {
            if (*this < rhs){
                return fixed_uint<width>(0);
            }
            if (*this == rhs) {
                return fixed_uint<width>(1);
            }
            fixed_uint<width> ret(0);
            for (size_t shift = upper_one_() - rhs.upper_one_();; --shift) {
                if (*this >= (rhs << shift)) {
                    *this -= (rhs << shift);
                    ret ^= (fixed_uint<width>(1) << shift);
                }
                if (shift == 0) {
                    break;
                }
            }
            return (*this = ret);
        }

        fixed_uint<width> operator%= (const fixed_uint<width>& rhs) noexcept {
            return (*this -= rhs * (*this / rhs));
        }

        std::string to_string() const noexcept {
            if (width <= 64) {
                return std::to_string(_data.to_ullong());
            }
            if (!_data.any()) {
                return "0";
            }
            std::string ret;
            ret.reserve((width + 2) / 3);
            fixed_uint<width> tmp(*this);
            for (fixed_uint<width> next(tmp / _base);; next = tmp / _base) {
                const std::string str(std::to_string(uint64_t(tmp - _base * next)));
                std::for_each(str.rbegin(), str.rend(), [&ret](const char x) {
                    ret += x;
                });
                if (next._data.any()) {
                    for (size_t i = str.size(); i < 19; ++i) {
                        ret += '0';
                    }
                    tmp = next;
                }
                else {
                    break;
                }
            }
            std::reverse(ret.begin(), ret.end());
            return ret;
        }

    };

}

#endif //  __STLB_FIXED_INT
