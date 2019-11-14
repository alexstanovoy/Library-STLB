#pragma once
#include <string>


namespace stlb {

    class bitvec {

    private:

        std::basic_string<uint8_t> _str;
        size_t _pos;

    public:

        bitvec() {
            _pos = 0;
        }

        bitvec(const uint8_t x) {
            _pos = 0;
            push_bit(x);
        }

        bitvec operator+ (const bitvec& x) const {
            bitvec ret;
            for (size_t i = 0; i < bit_size(); i++) {
                ret.push_bit(bit_at(i));
            }
            for (size_t i = 0; i < x.bit_size(); i++) {
                ret.push_bit(x.bit_at(i));
            }
            return ret;
        }

        bitvec operator+ (const uint8_t x) const {
            bitvec ret;
            for (size_t i = 0; i < bit_size(); i++) {
                ret.push_bit(bit_at(i));
            }
            ret.push_bit(x);
            return ret;
        }

        bitvec& operator+= (const bitvec& x) {
            for (size_t i = 0; i < x.bit_size(); i++) {
                push_bit(x.bit_at(i));
            }
            return *this;
        }

        bitvec& operator+= (const uint8_t x) {
            push_byte(x);
            return *this;
        }

        bitvec& push_bit(const uint8_t x) {
            if (_pos >= 8 * _str.size()) {
                _str.push_back(0);
            }
            _str[_pos / 8] &= ~(uint8_t(1) << (_pos % 8));
            _str[_pos / 8] ^= (x << (_pos % 8));
            _pos++;
            return *this;
        }

        bitvec& pop_bit() {
            _str[_pos / 8] &= (0xFF ^ (uint8_t(1) << (_pos % 8)));
            _pos--;
            if (_pos + 8 <= 8 * _str.size()) {
                _str.pop_back();
            }
            return *this;
        }

        bitvec& push_byte(const uint8_t x) {
            for (uint8_t i = 0; i < 8; i++) {
                push_bit((x >> i) & uint8_t(1));
            }
            return *this;
        }

        bitvec& pop_byte() {
            for (uint8_t i = 0; i < 8; i++) {
                pop_bit();
            }
            return *this;
        }

        uint8_t byte_from_bit(const size_t pos) const {
            uint8_t ret = 0;
            for (size_t i = 0; i < 8; i++) {
                ret ^= (bit_at(pos + i) << i);
            }
            return ret;
        }

        size_t byte_size() const {
            return _str.size();
        }

        size_t bit_size() const {
            return _pos;
        }

        uint8_t bit_at(const size_t pos) const {
            return (_str[pos / 8] >> (pos % 8)) & uint8_t(1);
        }

        uint8_t byte_at(const size_t pos) const {
            return _str[pos];
        }

        void clear() {
            _str.clear();
            _pos = 0;
        }

    };

}
