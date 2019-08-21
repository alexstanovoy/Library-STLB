#ifndef __STLB_FIB_HEAP
#define __STLB_FIB_HEAP 1

#include <cmath>
#include <vector>


namespace stlb
{

    template<class T>
    class fib_heap
    {

    private:

        struct node_t
        {

            T key;
            node_t* left;
            node_t* right;
            node_t* child;
            node_t* parent;
            size_t deg;
            bool mark;

            node_t(const T& key) : key(key) {
                left = this;
                right = this;
                child = nullptr;
                parent = nullptr;
                deg = 0;
                mark = false;
            }

        };

        node_t* _root = nullptr;
        size_t _size = 0;

        node_t *const merge_(node_t *const ptr1, node_t *const ptr2) const noexcept {
            if (ptr1 == nullptr) {
                return ptr2;
            }
            else if (ptr2 == nullptr) {
                return ptr1;
            }
            else {
                std::swap(ptr1->right->left, ptr2->right->left);
                std::swap(ptr1->right, ptr2->right);
                return (ptr1->key < ptr2->key ? ptr1 : ptr2);
            }
        }

        void cut_(node_t *const ptr) noexcept {
            if (ptr->parent != nullptr) {
                node_t *const L = ptr->left;
                node_t *const R = ptr->right;
                L->right = R;
                R->left = L;
                if (ptr->parent != nullptr) {
                    ptr->parent->deg--;
                    ptr->parent->child = (R == ptr ? nullptr : R);
                }
                ptr->left = ptr;
                ptr->right = ptr;
                ptr->parent = nullptr;
                _root = merge_(_root, ptr);
            }
            ptr->mark = false;
        }

        void cascading_cut_(node_t* ptr) noexcept {
            while (ptr != nullptr) {
                if (ptr->mark == false) {
                    ptr->mark = true;
                    break;
                }
                else {
                    node_t *const parent = ptr->parent;
                    cut_(ptr);
                    ptr = parent;
                }
            }
        }

        void consolidate_() noexcept {
            _root->left->right = nullptr;
            size_t max_size = std::floor(std::log(_size)/std::log((1+std::sqrt(5))/2.0));
            std::vector<node_t*> arr(max_size+2, nullptr);
            for (node_t* tmp = _root; tmp != nullptr;) {
                node_t *const next = tmp->right;
                tmp->left = tmp;
                tmp->right = tmp;
                tmp->parent = nullptr;
                while (arr[tmp->deg] != nullptr) {
                    node_t* up;
                    node_t* down;
                    if (tmp->key < arr[tmp->deg]->key) {
                        up = tmp, down = arr[tmp->deg];
                    }
                    else {
                        up = arr[tmp->deg], down = tmp;
                    }
                    arr[tmp->deg] = nullptr;
                    down->parent = up;
                    up->child = merge_(up->child, down);
                    up->deg++;
                    tmp = up;
                }
                arr[tmp->deg] = tmp;
                tmp = next;
            }
            _root = nullptr;
            for (node_t* tmp : arr) {
                _root = merge_(_root, tmp);
            }
        }

        void clear_(const node_t *const ptr) const noexcept {
            if (ptr != nullptr) {
                ptr->left->right = nullptr;
                const node_t* tmp = ptr;
                while (tmp != nullptr) {
                    const node_t* const next = tmp->right;
                    clear_(tmp->child);
                    delete tmp;
                    tmp = next;
                }
            }
        }

    public:

        class pointer
        {

        private:

            const node_t* _ptr;

        public:

            constexpr pointer() noexcept : _ptr(nullptr) {}

            pointer(node_t *const ptr) noexcept : _ptr(ptr) {}

            pointer& operator= (const pointer& other) noexcept {
                _ptr = other._ptr;
                return *this;
            }

            pointer& operator== (const pointer& other) const noexcept {
                return (_ptr == other._ptr);
            }

            const T& operator* () const noexcept {
                return _ptr->key;
            }

        };

        fib_heap() = default;

        ~fib_heap() noexcept {
            clear();
        }

        size_t size() const noexcept {
            return _size;
        }

        const T& top() const noexcept {
            return _root->key;
        }

        bool empty() const noexcept {
            return _size == 0;
        }

        void clear() noexcept {
            clear_(_root);
            _root = nullptr;
            _size = 0;
        }

        pointer push(const T& key) noexcept {
            node_t *const new_node = new node_t(key);
            _root = merge_(_root, new_node);
            _size++;
            return pointer(new_node);
        }

        void pop() noexcept {
            if (_size == 1) {
                delete _root;
                _root = nullptr;
            }
            else {
                merge_(_root, _root->child);
                node_t *const new_root = _root->right;
                _root->left->right = _root->right;
                _root->right->left = _root->left;
                delete _root;
                _root = new_root;
                consolidate_();
            }
            _size--;
        }

        void decrease_key(pointer& node, const T& key) noexcept {
            node.ptr->key = key;
            node_t *const parent = node.ptr->parent;
            if (parent != nullptr && key < parent->key) {
                cut_(node.ptr);
                cascading_cut_(parent);
            }
            else if (key < _root->key) {
                _root = node.ptr;
            }
        }
        
    };

}

#endif //  __STLB_FIB_HEAP
