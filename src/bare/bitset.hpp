#pragma once
#include <string>
#include <algorithm>

#include <cassert>


namespace stlb
{

    template<const size_t _width>
    class bitset
    {

    private:

        typedef unsigned long word_t;

        static constexpr const size_t _bits = 8 * sizeof(word_t);
        static constexpr const size_t _len = (_width + _bits - 1) / _bits;
        static constexpr const word_t _mask = (~static_cast<word_t>(0) >> ((_bits - (_width % _bits)) % _bits));

        word_t _data[_len] = {};

        inline size_t ctzl_(const word_t val) const noexcept {
            if (val == 0) {
                return _bits;
            }
            return __builtin_ctzl(val);
        }

        inline size_t clzl_(const word_t val) const noexcept {
            if (val == 0) {
                return _bits;
            }
            return __builtin_clzl(val);
        }

    public:

        class iterator : public std::iterator<
            std::random_access_iterator_tag,
            word_t
        > {

        private:

            friend class bitset;

            word_t* _ptr;

            constexpr iterator(word_t *const ptr) noexcept : _ptr(ptr) {}

        public:

            constexpr iterator() noexcept = default;

            constexpr iterator(const iterator& other) noexcept = default;

            ~iterator() noexcept = default;

            iterator& operator= (const iterator& other) noexcept = default;

            bool operator== (const iterator& other) const noexcept {
                return _ptr == other._ptr;
            }

            bool operator!= (const iterator& other) const noexcept {
                return _ptr != other._ptr;
            }

            bool operator< (const iterator& rhs) const noexcept {
                return _ptr < rhs._ptr;
            }

            bool operator<= (const iterator& rhs) const noexcept {
                return _ptr <= rhs._ptr;
            }

            bool operator> (const iterator& rhs) const noexcept {
                return _ptr > rhs._ptr;
            }

            bool operator>= (const iterator& rhs) const noexcept {
                return _ptr >= rhs._ptr;
            }

            word_t& operator* () noexcept {
                return *_ptr;
            }

            word_t operator* () const noexcept {
                return *_ptr;
            }

            iterator& operator++ () noexcept {
                ++_ptr;
                return *this;
            }

            iterator operator++ (int) noexcept {
                return iterator(_ptr++);
            }

            iterator& operator-- () noexcept {
                --_ptr;
                return *this;
            }

            iterator operator-- (int) noexcept {
                return iterator(_ptr--);
            }

            iterator& operator+= (const size_t val) noexcept {
                _ptr += val;
                return *this;
            }

            iterator& operator-= (const size_t val) noexcept {
                _ptr -= val;
                return *this;
            }

            iterator operator+ (const size_t val) const noexcept {
                return iterator(_ptr + val);
            }

            iterator operator- (const size_t val) const noexcept {
                return iterator(_ptr - val);
            }

            friend iterator operator+ (const size_t lhs, const iterator& rhs) noexcept {
                return iterator(rhs._ptr + lhs);
            }

            friend ptrdiff_t operator- (const iterator& lhs, const iterator& rhs) noexcept {
                return lhs._ptr - rhs._ptr;
            }

            word_t& operator[] (const ptrdiff_t ind) const noexcept {
                return *(_ptr + ind);
            }

        };

        class reference
        {

        private:

            friend class bitset;

            word_t *const _ptr;
            uint8_t _ind;

            constexpr reference(word_t *const ptr, const uint8_t ind) noexcept : _ptr(ptr), _ind(ind) {};

        public:

            operator bool () const noexcept {
                return (*_ptr >> _ind) & 1;
            }

            reference& operator= (const bool val) noexcept {
                *_ptr ^= (-static_cast<word_t>(val != 0) ^ *_ptr) & (static_cast<word_t>(1) << _ind);
                return *this;
            }

            bool operator~ () const noexcept {
                return !bool(*this);
            }

            reference& flip() noexcept {
                *_ptr ^= static_cast<word_t>(1) << _ind;
                return *this;
            }

        };

        constexpr bitset() noexcept = default;

        constexpr bitset(const bitset<_width>& other) noexcept = default;

        constexpr bitset(const unsigned long val) noexcept {
            _data[0] = val;
            normalize();
        }

        constexpr bitset(const unsigned long long val) noexcept {
            if (sizeof(word_t) == sizeof(unsigned long long)) {
                _data[0] = val;
            }
            else {
                _data[0] = static_cast<word_t>(val);
                _data[1] = static_cast<word_t>(val >> (_bits / 2));
            }
            normalize();
        }

        ~bitset() noexcept = default;

        constexpr iterator begin() const noexcept {
            return iterator(const_cast<word_t *const>(_data));
        }

        constexpr iterator end() const noexcept {
            return iterator(const_cast<word_t *const>(_data) + _len);
        }

        bitset<_width>& operator= (const bitset<_width>& rhs) noexcept = default;

        bitset<_width>& operator= (bitset<_width>&& rhs) noexcept {
            std::swap(_data, rhs._data);
            return *this;
        }

        inline void normalize() noexcept {
            _data[_len - 1] &= _mask;
        }

        bool all() const noexcept {
            for (size_t i = 0; i < _len; ++i) {
                if (_data[i] != ~static_cast<word_t>(0)) {
                    return false;
                }
            }
            return true;
        }

        bool any() const noexcept {
            for (size_t i = 0; i < _len; ++i) {
                if (_data[i] != static_cast<word_t>(0)) {
                    return true;
                }
            }
            return false;
        }

        bool none() const noexcept {
            return !any();
        }

        constexpr size_t size() const noexcept {
            return _len;
        }

        size_t count() const noexcept {
            size_t ret = 0;
            for (size_t i = 0; i < _len; ++i) {
                ret += __builtin_popcountl(_data[i]);
            }
            return ret;
        }

        bitset<_width>& flip() noexcept {
            for (size_t i = 0; i < _len; ++i) {
                _data[i] = ~_data[i];
            }
            normalize();
            return *this;
        }

        bitset<_width>& operator<<= (const size_t pos) {
            const size_t shift = pos / _bits;
            const size_t offset = pos % _bits;
            if (offset == 0) {
                for (ssize_t i = _len - 1; i >= static_cast<ssize_t>(shift); --i) {
                    _data[i] = _data[i - shift];
                }
            }
            else {
                const size_t offset_internal = _bits - offset;
                for (size_t i = _len - 1; i > shift; --i) {
                    _data[i] = (_data[i - shift] << offset) |
                               (_data[i - shift - 1] >> offset_internal);
                }
                _data[shift] = (_data[0] << offset);
            }
            std::fill(_data, _data + shift, static_cast<word_t>(0));
            normalize();
            return *this;
        }

        bitset<_width>& operator>>= (const size_t pos) noexcept {
            const size_t shift = pos / _bits;
            const size_t offset = pos % _bits;
            const size_t lim = _len - shift - 1;
            if (offset == 0) {
                for (size_t i = 0; i <= lim; ++i) {
                    _data[i] = _data[i + shift];
                }
            }
            else {
                const size_t offset_internal = _bits - offset;
                for (size_t i = 0; i < lim; ++i) {
                    _data[i] = (_data[i + shift] >> offset) |
                               (_data[i + shift + 1] << offset_internal);
                }
                _data[lim] = (_data[_len - 1] >> offset);
            }
            std::fill(_data + lim + 1, _data + _len, static_cast<word_t>(0));
            normalize();
            return *this;
        }

        bitset<_width>& operator^= (const bitset<_width>& rhs) noexcept {
            for (size_t i = 0; i < _len; ++i) {
                _data[i] ^= rhs._data[i];
            }
            return *this;
        }

        bitset<_width>& operator|= (const bitset<_width>& rhs) noexcept {
            for (size_t i = 0; i < _len; ++i) {
                _data[i] |= rhs._data[i];
            }
            return *this;
        }

        bitset<_width>& operator&= (const bitset<_width>& rhs) noexcept {
            for (size_t i = 0; i < _len; ++i) {
                _data[i] &= rhs._data[i];
            }
            return *this;
        }

        bitset<_width> operator<< (const size_t pos) const noexcept {
            return bitset<_width>(*this) <<= pos;
        }

        bitset<_width> operator>> (const size_t pos) const noexcept {
            return bitset<_width>(*this) >>= pos;
        }

        bitset<_width> operator~ () const noexcept {
            return bitset<_width>(*this).flip();
        }

        friend bitset<_width> operator^ (bitset<_width> lhs, const bitset<_width>& rhs) noexcept {
            return lhs ^= rhs;
        }

        friend bitset<_width> operator| (bitset<_width> lhs, const bitset<_width>& rhs) noexcept {
            return lhs |= rhs;
        }

        friend bitset<_width> operator& (bitset<_width> lhs, const bitset<_width>& rhs) noexcept {
            return lhs &= rhs;
        }

        reference operator[] (const size_t ind) const noexcept {
            return reference(const_cast<word_t *const>(&_data[ind / _bits]), ind % _bits);
        }

        friend bool operator== (const bitset<_width>& lhs, const bitset<_width>& rhs) noexcept {
            for (size_t i = 0; i < _len; ++i) {
                if (lhs._data[i] != rhs._data[i]) {
                    return false;
                }
            }
            return true;
        }

        friend bool operator!= (const bitset<_width>& lhs, const bitset<_width>& rhs) noexcept {
            return !(lhs._data == rhs._data);
        }

        bool test(const size_t ind) const noexcept {
            return ((_data[ind / _bits] >> (ind % _bits)) & 1);
        }

        bitset<_width>& flip(const size_t ind) noexcept {
            _data[ind / _bits] ^= static_cast<word_t>(1) << (ind % _bits);
            return *this;
        }

        bitset<_width>& set() noexcept {
            std::fill(_data, _data + _len, ~static_cast<word_t>(0));
            normalize();
            return *this;
        }

        bitset<_width>& set(const size_t ind, const bool val) noexcept {
            _data[ind / _bits] ^= (-static_cast<word_t>(val != 0) ^ _data[ind / _bits]) & (static_cast<word_t>(1) << (ind / _bits));
            return *this;
        }

        bitset<_width>& reset() noexcept {
            std::fill(_data, _data + _len, static_cast<word_t>(0));
            return *this;
        }

        bitset<_width>& reset(const size_t ind) noexcept {
            _data[ind / _bits] &= ~(static_cast<word_t>(1) << (ind % _bits));
            return *this;
        }

        word_t& get_word(const size_t pos) const noexcept {
            return _data[pos];
        }

        size_t next_one(size_t offset = 0) const noexcept {
            size_t ind = offset / _bits;
            offset %= _bits;
            const size_t cnt = ctzl_(_data[ind] & (~static_cast<word_t>(0) << offset));
            if (cnt != _bits) {
                return ind * _bits + cnt;
            }
            for (++ind; ind < _len && _data[ind] == 0; ++ind);
            if (ind == _len) {
                return _width;
            }
            return ind * _bits + ctzl_(_data[ind]);
        }

        size_t prev_one(size_t offset = _width - 1) const noexcept {
            ssize_t ind = offset / _bits;
            offset %= _bits;
            const size_t cnt = clzl_(_data[ind] & (~static_cast<word_t>(0) >> (_bits - offset - 1)));
            if (cnt != _bits) {
                return ind * _bits + _bits - cnt - 1;
            }
            for (--ind; ind >= 0 && _data[ind] == 0; --ind);
            if (ind == -1) {
                return _width;
            }
            return ind * _bits + _bits - clzl_(_data[ind]) - 1;
        }

        size_t next_zero(size_t offset = 0) const noexcept {
            size_t ind = offset / _bits;
            offset %= _bits;
            const size_t cnt = ctzl_(~_data[ind] & (~static_cast<word_t>(0) << offset));
            if (cnt != _bits) {
                return ind * _bits + cnt;
            }
            for (++ind; ind < _len && _data[ind] == ~static_cast<word_t>(0); ++ind);
            if (ind == _len) {
                return _width;
            }
            return ind * _bits + ctzl_(~_data[ind]);
        }

        size_t prev_zero(ssize_t offset = _width - 1) const noexcept {
            ssize_t ind = offset / _bits;
            offset %= _bits;
            const size_t cnt = clzl_(~_data[ind] & (~static_cast<word_t>(0) >> (_bits - offset - 1)));
            if (cnt != _bits) {
                return ind * _bits + _bits - cnt - 1;
            }
            for (--ind; ind >= 0 && _data[ind] == ~static_cast<word_t>(0); --ind);
            if (ind == -1) {
                return _width;
            }
            return ind * _bits + _bits - clzl_(~_data[ind]) - 1;
        }

        size_t first_diff(const bitset<_width>& other) const noexcept {
            for (size_t i = 0; i < _len; ++i) {
                if (_data[i] != other._data[i]) {
                    return i * _bits + ctzl_(_data[i] ^ other._data[i]);
                }
            }
            return _width;
        }

        size_t last_diff(const bitset<_width>& other) const noexcept {
            for (ssize_t i = _len - 1; i >= 0; --i) {
                if (_data[i] != other._data[i]) {
                    return i * _bits + _bits - clzl_(_data[i] ^ other._data[i]) - 1;
                }
            }
            return _width;
        }

        uint32_t to_uint32_t() const noexcept {
            return static_cast<uint32_t>(_data[0]);
        }

        uint64_t to_uint64_t() const noexcept {
            if (sizeof(word_t) == 32) {
                return (static_cast<uint64_t>(_data[1]) << 32) + _data[0];
            }
            return static_cast<uint64_t>(_data[0]); 
        }

        std::string to_string() const noexcept {
            std::string ret;
            ret.resize(_width);
            for (size_t i = 0; i < _width; ++i) {
                ret[_width - i - 1] = static_cast<char>(test(i) + '0');
            }
            return ret;
        }

        friend std::ostream& operator<< (std::ostream& os, const bitset<_width>& val) noexcept {
            os << val.to_string();
            return os;
        }

    };

}
