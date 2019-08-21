#ifndef __STLB_HUFFMAN
#define __STLB_HUFFMAN 1

#include "bitvec.h"
#include <vector>
#include <queue>


namespace stlb
{

    class huffman
    {

    private:

        struct _node
        {

            const byte symb;
            const int64_t count;
            _node* L;
            _node* R;

            _node(const byte symb, const int64_t count,
                  _node *const L, _node *const R) : symb(symb), count(count), L(L), R(R) {

            }

        };

        _node* _root;
        stlb::bitvec _symb_code[0x100];

        struct ptr_comp_ {
            bool operator() (const _node *const a, const _node *const b) const {
                return a->count > b->count;
            }
        };

        void clear_(_node *const ptr) {
            if (ptr != nullptr) {
                clear_(ptr->L);
                clear_(ptr->R);
                delete ptr;
            }
        }

        bool isleaf_(const _node *const ptr) const {
            return ptr->L == nullptr && ptr->R == nullptr;
        }

        std::vector<size_t> byte_count_(const stlb::bitvec& file) const {
            std::vector<size_t> ret(0x100, 0);
            for (size_t i = 0; i < file.byte_size(); i++) {
                ret[file.byte_at(i)]++;
            }
            return ret;
        }

        std::priority_queue<_node*, std::vector<_node*>, ptr_comp_> create_queue_(const stlb::bitvec& file) const {
            std::vector<size_t> count = byte_count_(file);
            std::priority_queue<_node*, std::vector<_node*>, ptr_comp_> ret;
            for (byte i = 0;; i++) {
                if (count[i] != 0) {
                    ret.push(new _node(i, count[i], nullptr, nullptr));
                }
                if (i == 0xFF) {
                    break;
                }
            }
            return ret;
        }

        void create_bytes_(const _node *const ptr, stlb::bitvec cur = stlb::bitvec()) {
            if (isleaf_(ptr)) {
                _symb_code[ptr->symb] = cur;
            }
            else {
                create_bytes_(ptr->L, cur + stlb::bitvec(0));
                create_bytes_(ptr->R, cur + stlb::bitvec(1));
            }
        }

        stlb::bitvec encode_tree_(const _node *const ptr) const {
            stlb::bitvec ret;
            if (isleaf_(ptr)) {
                ret.push_bit(1);
                ret.push_byte(ptr->symb);
            }
            else {
                ret.push_bit(0);
                ret += encode_tree_(ptr->L);
                ret += encode_tree_(ptr->R);
            }
            return ret;
        }

        size_t read_tree_from_file_(_node*& ptr, const stlb::bitvec& file, size_t ind = 3) {
            byte bit = file.bit_at(ind++);
            if (bit == 1) {
                ptr = new _node(file.byte_from_bit(ind), 0, nullptr, nullptr);
                return ind + 8;
            }
            else {
                ptr = new _node(0, 0, nullptr, nullptr);
                ind = read_tree_from_file_(ptr->L, file, ind);
                ind = read_tree_from_file_(ptr->R, file, ind);
                return ind;
            }
        }

        size_t read_tree_from_file_(const stlb::bitvec& file) {
            const size_t ret = read_tree_from_file_(_root, file);
            create_bytes_(_root);
            return ret;
        }

        void make_tree_from_file_(const stlb::bitvec& file) {
            std::priority_queue<_node*, std::vector<_node*>, ptr_comp_> q = create_queue_(file);
            while (q.size() > 2) {
                _node *const first  = q.top(); q.pop();
                _node *const second = q.top(); q.pop();
                q.push(new _node(0, first->count + second->count, first, second));
            }
            if (q.size() == 1) {
                _root = new _node(0, 0, q.top(), new _node(0, 0, nullptr, nullptr)); q.pop();
            }
            else if (q.size() == 2) {
                _node *const first  = q.top(); q.pop();
                _node *const second = q.top(); q.pop();
                _root = new _node(0, 0, first, second);
            }
            create_bytes_(_root);
        }

    public:

        stlb::bitvec encode(const stlb::bitvec& file) {
            make_tree_from_file_(file);
            stlb::bitvec ret = encode_tree_(_root);
            for (size_t i = 0; i < file.byte_size(); i++) {
                ret += _symb_code[file.byte_at(i)];
            }
            clear_(_root);
            byte zero = (8 - (ret.bit_size() + 3) % 8) % 8;
            for (size_t i = 0; i < zero; i++) {
                ret.push_bit(0);
            }
            return stlb::bitvec((zero >> 2) & 1) + stlb::bitvec((zero >> 1) & 1) + stlb::bitvec((zero >> 0) & 1) + ret;
        }

        stlb::bitvec decode(const stlb::bitvec& file) {
            byte zero = 4 * file.bit_at(0) + 2 * file.bit_at(1) + 1 * file.bit_at(2);
            stlb::bitvec ret;
            size_t i = read_tree_from_file_(file);
            const _node* ptr = _root;
            for (; i < file.bit_size() - zero; i++) {
                ptr = (file.bit_at(i) == 0 ? ptr->L : ptr->R);
                if (isleaf_(ptr)) {
                    ret += ptr->symb;
                    ptr = _root;
                }
            }
            clear_(_root);
            return ret;
        }

    };

}

#endif //  __STLB_HUFFMAN
