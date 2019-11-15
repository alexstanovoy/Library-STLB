#pragma once
#include <string>
#include <cstddef>
#include <cassert>
#include <algorithm>


namespace stlb
{

    namespace data_structure
    {

        
        /**
         * @brief      This class describes a bitset data structure. Width must
         *             be known at compile time.
         *
         * @tparam     _width  Defines a width of the bitset.
         */
        template<const size_t _width>
        class bitset
        {

        private:

            typedef uint64_t word_t;

            static constexpr const size_t _bits = 64;
            static constexpr const size_t _len = (_width + _bits - 1) / _bits;
            static constexpr const word_t _mask = (~static_cast<word_t>(0) >> ((_bits - (_width % _bits)) % _bits));

            word_t _data[_len] = {};

            /**
             * @brief      Returns count of trailing zeros. Uses __builtin_ctzl,
             *             but because calling __builtin_ctzl(0) is UB in GCC,
             *             zero is special case.
             *
             * @param[in]  val   Input number.
             *
             * @return     Returns count of trailing zeros.
             */
            size_t ctzl_(const word_t val) const noexcept {
                if (val == 0) {
                    return _bits;
                }
                return __builtin_ctzll(val);
            }

            /**
             * @brief      Returns count of leading zeros. Uses __builtin_clzl,
             *             but because calling __builtin_clzl(0) is UB in GCC,
             *             zero is special case.
             *
             * @param[in]  val   Input number.
             *
             * @return     Returns count of leading zeros.
             */
            size_t clzl_(const word_t val) const noexcept {
                if (val == 0) {
                    return _bits;
                }
                return __builtin_clzll(val);
            }

        public:

            /**
             * @brief      Describes a forward iterator over words of bitset.
             *             Note that every assigment of last word of bitset
             *             should be followed by calling normalize function.
             */
            class iterator : public std::iterator<
                std::random_access_iterator_tag,
                word_t
            > {

            private:

                friend class bitset;

                word_t* _ptr;

                /**
                 * @brief      Constructs a new instance.
                 *
                 * @param      ptr   The pointer to the current position.
                 */
                constexpr iterator(word_t *const ptr) noexcept : _ptr(ptr) {}

            public:

                /**
                 * @brief      Constructs a new empty instance.
                 */
                constexpr iterator() noexcept = default;

                /**
                 * @brief      Constructs a new instance based on other iterator.
                 *
                 * @param[in]  other  Other iterator.
                 */
                constexpr iterator(const iterator& other) noexcept = default;

                /**
                 * @brief      Destroys the object.
                 */
                ~iterator() noexcept = default;

                /**
                 * @brief      Assignment operator.
                 *
                 * @param[in]  other  Other iterator.
                 *
                 * @return     The result of the assignment.
                 */
                iterator& operator= (const iterator& other) noexcept = default;

                /**
                 * @brief      Equality operator.
                 *
                 * @param[in]  other  Other iterator.
                 *
                 * @return     The result of the equality.
                 */
                bool operator== (const iterator& other) const noexcept {
                    return _ptr == other._ptr;
                }

                /**
                 * @brief      Inequality operator.
                 *
                 * @param[in]  other  Other iterator.
                 *
                 * @return     The result of the inequality.
                 */
                bool operator!= (const iterator& other) const noexcept {
                    return _ptr != other._ptr;
                }

                /**
                 * @brief      Less-than comparison operator.
                 *
                 * @param[in]  rhs   The right hand side.
                 *
                 * @return     The result of the less-than comparison.
                 */
                bool operator< (const iterator& rhs) const noexcept {
                    return _ptr < rhs._ptr;
                }

                /**
                 * @brief      Less-than-or-equal comparison operator.
                 *
                 * @param[in]  rhs   The right hand side.
                 *
                 * @return     The result of the less-than-or-equal comparison.
                 */
                bool operator<= (const iterator& rhs) const noexcept {
                    return _ptr <= rhs._ptr;
                }

                /**
                 * @brief      Greater-than comparison operator.
                 *
                 * @param[in]  rhs   The right hand side.
                 *
                 * @return     The result of the greater-than comparison/
                 */
                bool operator> (const iterator& rhs) const noexcept {
                    return _ptr > rhs._ptr;
                }

                /**
                 * @brief      Greater-than-or-equal comparison operator.
                 *
                 * @param[in]  rhs   The right hand side.
                 *
                 * @return     The result of the greater-than-or-equal comparison.
                 */
                bool operator>= (const iterator& rhs) const noexcept {
                    return _ptr >= rhs._ptr;
                }

                /**
                 * @brief      Word access operator.
                 *
                 * @return     The reference to word.
                 */
                word_t& operator* () noexcept {
                    return *_ptr;
                }

                /**
                 * @brief      Const word access operator.
                 *
                 * @return     The word.
                 */
                word_t operator* () const noexcept {
                    return *_ptr;
                }

                /**
                 * @brief      Prefix increment operator.
                 *
                 * @return     The next word of bitset.
                 */
                iterator& operator++ () noexcept {
                    ++_ptr;
                    return *this;
                }

                /**
                 * @brief      Suffix increment operator.
                 *
                 * @return     Current iterator.
                 */
                iterator operator++ (int) noexcept {
                    return iterator(_ptr++);
                }

                /**
                 * @brief      Prefix decrement operator.
                 *
                 * @return     The previous word of bitset.
                 */
                iterator& operator-- () noexcept {
                    --_ptr;
                    return *this;
                }

                /**
                 * @brief      Suffix decrement operator.
                 *
                 * @return     Current iterator.
                 */
                iterator operator-- (int) noexcept {
                    return iterator(_ptr--);
                }

                /**
                 * @brief      Addition assignment operator.
                 *
                 * @param[in]  val   Step size.
                 *
                 * @return     The result of the addition assignment.
                 */
                iterator& operator+= (const size_t val) noexcept {
                    _ptr += val;
                    return *this;
                }

                /**
                 * @brief      Subtraction assignment operator.
                 *
                 * @param[in]  val   Step size.
                 *
                 * @return     The result of the subtraction assignment.
                 */
                iterator& operator-= (const size_t val) noexcept {
                    _ptr -= val;
                    return *this;
                }

                /**
                 * @brief      Addition operator.
                 *
                 * @param[in]  val   Step size.
                 *
                 * @return     The result of the addition.
                 */
                iterator operator+ (const size_t val) const noexcept {
                    return iterator(_ptr + val);
                }

                /**
                 * @brief      Subtraction operator.
                 *
                 * @param[in]  val   Step size.
                 *
                 * @return     The result of the subtraction.
                 */
                iterator operator- (const size_t val) const noexcept {
                    return iterator(_ptr - val);
                }

                /**
                 * @brief      Addition operator.
                 *
                 * @param[in]  lhs   Step size.
                 * @param[in]  rhs   The iterator.
                 *
                 * @return     The result of the addition.
                 */
                friend iterator operator+ (const size_t lhs, const iterator& rhs) noexcept {
                    return iterator(rhs._ptr + lhs);
                }

                /**
                 * @brief      Subtraction operator.
                 *
                 * @param[in]  lhs   Step size.
                 * @param[in]  rhs   The iterator.
                 *
                 * @return     The result of the subtraction.
                 */
                friend ptrdiff_t operator- (const iterator& lhs, const iterator& rhs) noexcept {
                    return lhs._ptr - rhs._ptr;
                }

                /**
                 * @brief      Indexer operator.
                 *
                 * @param[in]  val   Step size.
                 *
                 * @return     The result of the indexer.
                 */
                word_t& operator[] (const ptrdiff_t val) const noexcept {
                    return *(_ptr + val);
                }

            };

            /**
             * @brief      A reference to the bit on bitset.
             */
            class reference
            {

            private:

                friend class bitset;

                word_t *const _ptr;
                uint8_t _ind;

                /**
                 * @brief      Constructs a new instance.
                 *
                 * @param      ptr   The pointer
                 * @param[in]  ind   The index of bit in word.
                 */
                constexpr reference(word_t *const ptr, const uint8_t ind) noexcept : _ptr(ptr), _ind(ind) {};

            public:

                /**
                 * @brief      Bool conversion operator.
                 */
                operator bool () const noexcept {
                    return (*_ptr >> _ind) & 1;
                }

                /**
                 * @brief      Assignment operator.
                 *
                 * @param[in]  val   The value.
                 *
                 * @return     The result of the assignment.
                 */
                reference& operator= (const bool val) noexcept {
                    *_ptr ^= (-static_cast<word_t>(val != 0) ^ *_ptr) & (static_cast<word_t>(1) << _ind);
                    return *this;
                }

                /**
                 * @brief      Bitwise 'one's complement' operator.
                 *
                 * @return     The result of the bit 'one's complement'.
                 */
                bool operator~ () const noexcept {
                    return !bool(*this);
                }

                /**
                 * @brief      Flips the bit
                 *
                 * @return     The result of the bit 'one's complement'.
                 */
                reference& flip() noexcept {
                    *_ptr ^= (static_cast<word_t>(1) << _ind);
                    return *this;
                }

            };

            /**
             * @brief      Constructs a new instance.
             */
            constexpr bitset() noexcept = default;

            /**
             * @brief      Constructs a new instance basing on other.
             *
             * @param[in]  other  The other
             */
            constexpr bitset(const bitset<_width>& other) noexcept = default;

            /**
             * @brief      Constructs a new instance basing on uint64_t.
             *
             * @param[in]  val   The value
             */
            constexpr bitset(const uint64_t val) noexcept {
                _data[0] = val;
                normalize();
            }

            /**
             * @brief      Destroys the object.
             */
            ~bitset() noexcept = default;

            /**
             * @brief      Returns iterator to the beginning word of bitset.
             *
             * @return     Iterator to the begin of the bitset.
             */
            constexpr iterator begin() const noexcept {
                return iterator(const_cast<word_t *const>(_data));
            }

            /**
             * @brief      Returns iterator to the ending word of bitset.
             *
             * @return     Iterator to the end of the bitset.
             */
            constexpr iterator end() const noexcept {
                return iterator(const_cast<word_t *const>(_data) + _len);
            }

            /**
             * @brief      Assignment operator.
             *
             * @param[in]  rhs   The right hand side.
             *
             * @return     The result of the assignment.
             */
            bitset<_width>& operator= (const bitset<_width>& rhs) noexcept = default;

            /**
             * @brief      Move ssignment operator.
             *
             * @param      rhs   The right hand side
             *
             * @return     The result of the assignment
             */
            bitset<_width>& operator= (bitset<_width>&& rhs) noexcept {
                std::swap(_data, rhs._data);
                return *this;
            }

            /**
             * @brief      Normalizes exceeding bits of bitset by setting them to 0.
             */
            void normalize() noexcept {
                _data[_len - 1] &= _mask;
            }

            /**
             * @brief      Checks are all bits of bitset set to 1.
             *
             * @return     ***true*** if all bits of bitset are set to 1
             *             or ***false*** if not.
             */
            bool all() const noexcept {
                for (size_t i = 0; i < _len; ++i) {
                    if (_data[i] != ~static_cast<word_t>(0)) {
                        return false;
                    }
                }
                return true;
            }

            /**
             * @brief      Checks is any bit of bitset set to 1.
             *
             * @return     ***true*** if any bit of bitset is set to 1
             *             or ***false*** if not.
             */
            bool any() const noexcept {
                for (size_t i = 0; i < _len; ++i) {
                    if (_data[i] != static_cast<word_t>(0)) {
                        return true;
                    }
                }
                return false;
            }

            /**
             * @brief      Checks are all bits of bitset set to 0.
             *
             * @return     ***true*** if all bits of bitset are set to 0
             *             or ***false*** if not.
             */
            bool none() const noexcept {
                return !any();
            }

            /**
             * @brief      Returns the width of bitset.
             *
             * @return     Width of bitset.
             */
            constexpr size_t size() const noexcept {
                return _width;
            }

            /**
             * @brief      Returns the count of 1 bits in bitset.
             *
             * @return     Count of 1 bits in bitset.
             */
            size_t count() const noexcept {
                size_t ret = 0;
                for (size_t i = 0; i < _len; ++i) {
                    ret += __builtin_popcountl(_data[i]);
                }
                return ret;
            }

            /**
             * @brief      Flips the values in bitset.
             *
             * @return     This bitset.
             */
            bitset<_width>& flip() noexcept {
                for (size_t i = 0; i < _len; ++i) {
                    _data[i] = ~_data[i];
                }
                normalize();
                return *this;
            }


            /**
             * @brief      Bitwise left shift assignment operator.
             *
             * @param[in]  pos   The shift value.
             *
             * @return     The result of the bitwise left shift assignment.
             */
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

            /**
             * @brief      Bitwise right shift assignment operator.
             *
             * @param[in]  pos   The shift value.
             *
             * @return     The result of the bitwise right shift assignment.
             */
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

            /**
             * @brief      Bitwise 'exclusive or' assignment operator.
             *
             * @param[in]  rhs   The right bitset.
             *
             * @return     The result of the bitwise 'exclusive or' assignment.
             */
            bitset<_width>& operator^= (const bitset<_width>& rhs) noexcept {
                for (size_t i = 0; i < _len; ++i) {
                    _data[i] ^= rhs._data[i];
                }
                return *this;
            }

            /**
             * @brief      Bitwise 'or' assignment operator.
             *
             * @param[in]  rhs   The right bitset.
             *
             * @return     The result of the bitwise 'or' assignment.
             */
            bitset<_width>& operator|= (const bitset<_width>& rhs) noexcept {
                for (size_t i = 0; i < _len; ++i) {
                    _data[i] |= rhs._data[i];
                }
                return *this;
            }

            /**
             * @brief      Bitwise 'and' assignment operator.
             *
             * @param[in]  rhs   The right bitset.
             *
             * @return     The result of the bitwise 'and' assignment
             */
            bitset<_width>& operator&= (const bitset<_width>& rhs) noexcept {
                for (size_t i = 0; i < _len; ++i) {
                    _data[i] &= rhs._data[i];
                }
                return *this;
            }

            /**
             * @brief      Bitwise left shift operator.
             *
             * @param[in]  pos   The shift value.
             *
             * @return     The result of the bitwise left shift.
             */
            bitset<_width> operator<< (const size_t pos) const noexcept {
                return bitset<_width>(*this) <<= pos;
            }

            /**
             * @brief      Bitwise right shift operator.
             *
             * @param[in]  pos   The shift value.
             *
             * @return     The result of the bitwise right shift.
             */
            bitset<_width> operator>> (const size_t pos) const noexcept {
                return bitset<_width>(*this) >>= pos;
            }

            /**
             * @brief      Returns copy of bitset with flipped bits.
             *
             * @return     The result of the bitwise 'one's complement' on
             *             bitset.
             */
            bitset<_width> operator~ () const noexcept {
                return bitset<_width>(*this).flip();
            }

            /**
             * @brief      Bitwise XOR operator.
             *
             * @param[in]  lhs   Left bitset.
             * @param[in]  rhs   Right bitset.
             *
             * @return     The result of the XOR operator.
             */
            friend bitset<_width> operator^ (bitset<_width> lhs, const bitset<_width>& rhs) noexcept {
                return lhs ^= rhs;
            }

            /**
             * @brief      Bitwise OR operator.
             *
             * @param[in]  lhs   Left bitset.
             * @param[in]  rhs   Right bitset.
             *
             * @return     The result of the OR operator.
             */
            friend bitset<_width> operator| (bitset<_width> lhs, const bitset<_width>& rhs) noexcept {
                return lhs |= rhs;
            }

            /**
             * @brief      Bitwise AND operator.
             *
             * @param[in]  lhs   Left bitset.
             * @param[in]  rhs   Right bitset.
             *
             * @return     The result of the AND operator.
             */
            friend bitset<_width> operator& (bitset<_width> lhs, const bitset<_width>& rhs) noexcept {
                return lhs &= rhs;
            }

            /**
             * @brief      Bitset indexer operator.
             *
             * @param[in]  ind   The index.
             *
             * @return     The value of bit in position ind.
             */
            reference operator[] (const size_t ind) const noexcept {
                return reference(const_cast<word_t *const>(&_data[ind / _bits]), ind % _bits);
            }

            /**
             * @brief      Equality comparison operator.
             *
             * @param[in]  lhs   Left bitset.
             * @param[in]  rhs   Right bitset.
             *
             * @return     The result of the equality comparison.
             */
            friend bool operator== (const bitset<_width>& lhs, const bitset<_width>& rhs) noexcept {
                for (size_t i = 0; i < _len; ++i) {
                    if (lhs._data[i] != rhs._data[i]) {
                        return false;
                    }
                }
                return true;
            }

            /**
             * @brief      Inequality comparison operator.
             *
             * @param[in]  lhs   Left bitset.
             * @param[in]  rhs   Right bitset.
             *
             * @return     The result of the inequality comparison.
             */
            friend bool operator!= (const bitset<_width>& lhs, const bitset<_width>& rhs) noexcept {
                return !(lhs._data == rhs._data);
            }

            /**
             * @brief      Returns the bit value.
             *
             * @param[in]  ind   The index of bit.
             *
             * @return     Bit value.
             */
            bool test(const size_t ind) const noexcept {
                return ((_data[ind / _bits] >> (ind % _bits)) & 1);
            }

            /**
             * @brief      Flips the bit value.
             *
             * @param[in]  ind   The index of bit.
             *
             * @return     This bitset.
             */
            bitset<_width>& flip(const size_t ind) noexcept {
                _data[ind / _bits] ^= static_cast<word_t>(1) << (ind % _bits);
                return *this;
            }

            /**
             * @brief      Sets all bits values to 1.
             *
             * @return     This bitset.
             */
            bitset<_width>& set() noexcept {
                std::fill(_data, _data + _len, ~static_cast<word_t>(0));
                normalize();
                return *this;
            }

            /**
             * @brief      Sets bit value to provided.
             *
             * @param[in]  ind   The index.
             * @param[in]  val   The new value.
             *
             * @return     This bitset.
             */
            bitset<_width>& set(const size_t ind, const bool val) noexcept {
                _data[ind / _bits] ^= (-static_cast<word_t>(val != 0) ^ _data[ind / _bits]) & (static_cast<word_t>(1) << (ind / _bits));
                return *this;
            }

            /**
             * @brief      Sets bits values to 0.
             *
             * @return     This bitset,
             */
            bitset<_width>& reset() noexcept {
                std::fill(_data, _data + _len, static_cast<word_t>(0));
                return *this;
            }

            /**
             * @brief      Sets bit value to 0.
             *
             * @param[in]  ind   The index.
             *
             * @return     This bitset.
             */
            bitset<_width>& reset(const size_t ind) noexcept {
                _data[ind / _bits] &= ~(static_cast<word_t>(1) << (ind % _bits));
                return *this;
            }

            /**
             * @brief      Gets the word.
             *
             * @param[in]  pos   The position
             *
             * @return     The word.
             */
            word_t& get_word(const size_t pos) const noexcept {
                return _data[pos];
            }

            /**
             * @brief      Returns next 1-bit in bitset.
             *
             * @param[in]  offset  The offset.
             *
             * @return     First 1-bit index greater or equal to offset or width
             *             if there's no such bit.
             */
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

            /**
             * @brief      Returns previous 1-bit in bitset.
             *
             * @param[in]  offset  The offset.
             *
             * @return     First 1-bit index less or equal to offset or width
             *             if there's no such bit.
             */
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

            /**
             * @brief      Returns next 0-bit in bitset.
             *
             * @param[in]  offset  The offset.
             *
             * @return     First 0-bit index greater or equal to offset or width
             *             if there's no such bit.
             */
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

            /**
             * @brief      Returns previous 0-bit in bitset.
             *
             * @param[in]  offset  The offset.
             *
             * @return     First 0-bit index less or equal to offset or width
             *             if there's no such bit.
             */
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

            /**
             * @brief      Returns first bit where values of bitsets are
             *             different.
             *
             * @param[in]  other  The other bitset.
             *
             * @return     Index of first different bit.
             */
            size_t first_diff(const bitset<_width>& other) const noexcept {
                for (size_t i = 0; i < _len; ++i) {
                    if (_data[i] != other._data[i]) {
                        return i * _bits + ctzl_(_data[i] ^ other._data[i]);
                    }
                }
                return _width;
            }

            /**
             * @brief      Returns last bit where values of bitsets are
             *             different.
             *
             * @param[in]  other  The other bitset.
             *
             * @return     Index of last different bit.
             */
            size_t last_diff(const bitset<_width>& other) const noexcept {
                for (ssize_t i = _len - 1; i >= 0; --i) {
                    if (_data[i] != other._data[i]) {
                        return i * _bits + _bits - clzl_(_data[i] ^ other._data[i]) - 1;
                    }
                }
                return _width;
            }

            /**
             * @brief      Returns first word casted to uint32_t.
             *
             * @return     uint32_t cast.
             */
            uint32_t to_uint32_t() const noexcept {
                return static_cast<uint32_t>(_data[0]);
            }

            /**
             * @brief      Returns first word casted to uint64_t.
             *
             * @return     uint64_t cast.
             */
            uint64_t to_uint64_t() const noexcept {
                return _data[0];
            }

            /**
             * @brief      Returns a string representation of the bitset.
             *
             * @return     String representation of the bitset.
             */
            std::string to_string() const noexcept {
                std::string ret;
                ret.resize(_width);
                for (size_t i = 0; i < _width; ++i) {
                    ret[_width - i - 1] = static_cast<char>(test(i) + '0');
                }
                return ret;
            }

            /**
             * @brief      Puts string representation of the object to
             *             std::ostream.
             *
             * @param      os    std::ostream.
             * @param[in]  val   The bitset.
             *
             * @return     std::ostream.
             */
            friend std::ostream& operator<< (std::ostream& os, const bitset<_width>& val) noexcept {
                os << val.to_string();
                return os;
            }

        };

    }

}
