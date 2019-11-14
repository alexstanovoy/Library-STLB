#pragma once


namespace stlb
{


    template<typename T>
    class set
    {

    private:

        struct tree_node_t
        {
            T key;
            size_t size;
            int8_t height;
            tree_node_t* L;
            tree_node_t* R;

            tree_node_t(const T& key, tree_node_t *const L, tree_node_t *const R) : key(key), L(L), R(R) {
                size = 1;
                height = 1;
            }
        };

        tree_node_t* _root;

        int8_t height_(const tree_node_t *const ptr) const {
            return (ptr == nullptr ? 0 : ptr->height);
        }

        size_t size_(const tree_node_t *const ptr) const {
            return (ptr == nullptr ? 0 : ptr->size);
        }

        void fix_(tree_node_t *const ptr) {
            if (ptr != nullptr) {
                const int8_t heightl = height_(ptr->L);
                const int8_t heightr = height_(ptr->R);
                ptr->height = (heightl < heightr ? heightr : heightl) + 1;
                ptr->size = size_(ptr->L) + 1 + size_(ptr->R);
            }
        }

        const int8_t diff_(const tree_node_t *const ptr) const {
            return (ptr == nullptr ? 0 : height_(ptr->R) - height_(ptr->L));
        }

        const tree_node_t *const findmin_(tree_node_t *const ptr) const {
            return (ptr->L == nullptr ? ptr : findmin_(ptr->L));
        }

        tree_node_t *const erasemin_(tree_node_t *const ptr) {
            if (ptr->L == nullptr) {
                tree_node_t *const ret = ptr->R;
                delete ptr;
                return ret;
            }
            ptr->L = erasemin_(ptr->L);
            fix_(ptr);
            return balance_(ptr);
        }

        tree_node_t *const rightrotation_(tree_node_t *const ptr1) {
            tree_node_t *const ptr2 = ptr1->L;
            ptr1->L = ptr2->R;
            ptr2->R = ptr1;
            fix_(ptr1);
            fix_(ptr2);
            return ptr2;
        }

        tree_node_t *const leftrotation_(tree_node_t *const ptr2) {
            tree_node_t *const ptr1 = ptr2->R;
            ptr2->R = ptr1->L;
            ptr1->L = ptr2;
            fix_(ptr2);
            fix_(ptr1);
            return ptr1;
        }

        tree_node_t *const balance_(tree_node_t *const ptr) {
            fix_(ptr);
            if (diff_(ptr) > 1) {
                if (diff_(ptr->R) < 0) {
                    ptr->R = rightrotation_(ptr->R);
                }
                return leftrotation_(ptr);
            }
            else if (diff_(ptr) < -1) {
                if (diff_(ptr->L) > 0) {
                    ptr->L = leftrotation_(ptr->L);
                }
                return rightrotation_(ptr);
            }
            return ptr;
        }

        tree_node_t *const insert_(tree_node_t *const ptr, const T& x) {
            if (ptr == nullptr) {
                return new tree_node_t(x, nullptr, nullptr);
            }
            else if (x < ptr->key) {
                ptr->L = insert_(ptr->L, x);
            }
            else if (x == ptr->key) {
                return ptr;
            }
            else {
                ptr->R = insert_(ptr->R, x);
            }
            return balance_(ptr);
        }

        tree_node_t *const erase_(tree_node_t *const ptr, const T& x) {
            if (x < ptr->key) {
                ptr->L = erase_(ptr->L, x);
            }
            else if (x == ptr->key) {
                if (ptr->L == nullptr || ptr->R == nullptr) {
                    tree_node_t *const temp = (ptr->L == nullptr ? ptr->R : ptr->L);
                    delete ptr;
                    return temp;
                }
                else {
                    const tree_node_t *const min = findmin_(ptr->R);
                    ptr->key = min->key;
                    ptr->R = erasemin_(ptr->R);
                }
            }
            else {
                ptr->R = erase_(ptr->R, x);
            }
            return balance_(ptr);
        }

        void clear_(const tree_node_t *const ptr) {
            if (ptr != nullptr) {
                clear_(ptr->L);
                clear_(ptr->R);
                delete ptr;
            }
        }

        tree_node_t *const copy_(const tree_node_t *const ptr) const {
            if (ptr == nullptr) {
                return nullptr;
            }
            tree_node_t *const ret = new tree_node_t(ptr->key, copy_(ptr->L), copy_(ptr->R));
            fix_(ret);
            return ret;
        }

    public:

        set() {
            _root = nullptr;
        }

        set(const set& other) {
            _root = copy_(other._root);
        }

        ~set() {
            clear_(_root);
        }

        set& operator= (const set& other) {
            clear_(_root);
            _root = copy_(other._root);
        }

        void clear() {
            clear_(_root);
            _root = nullptr;
        }

        const T& find_by_order(size_t nth) {
            const tree_node_t* ptr = _root;
            while (nth != size_(ptr->L)) {
                if (nth < size_(ptr->L)) {
                    ptr = ptr->L;
                }
                else {
                    nth -= size_(ptr->L) + 1;
                    ptr = ptr->R;
                }
            }
            return ptr->key;
        }

        size_t order_of_key(const T& x) const {
            if (_root == nullptr) {
                return 0;
            }
            size_t order = 0;
            const tree_node_t* ptr = _root;
            while (ptr != nullptr) {
                if (x < ptr->key) {
                    ptr = ptr->L;
                }
                else if (x == ptr->key) {
                    return order;
                }
                else {
                    order += size_(ptr->L) + 1;
                    ptr = ptr->R;
                }
            }
            return order;
        }

        bool find(const T& x) const {
            const tree_node_t* ptr = _root;
            while (ptr != nullptr) {
                if (x < ptr->key) {
                    ptr = ptr->L;
                }
                else if (x == ptr->key) {
                    return true;
                }
                else {
                    ptr = ptr->R;
                }
            }
            return false;
        }

        const T& lower_bound(const T& x) const {
            const tree_node_t* ptr = _root;
            const T* ret = nullptr;
            while (ptr != nullptr) {
                if (x < ptr->key) {
                    ret = &ptr->key;
                    ptr = ptr->L;
                }
                else if (x == ptr->key) {
                    return *ptr->key;
                }
                else {
                    ptr = ptr->R;
                }
            }
            return ret;
        }

        const T& upper_bound(const T& x) const {
            const tree_node_t* ptr = _root;
            const T* ret = nullptr;
            while (ptr != nullptr) {
                if (x < ptr->key) {
                    ret = &ptr->key;
                    ptr = ptr->L;
                }
                else {
                    ptr = ptr->R;
                }
            }
            return *ret;
        }

        void insert(const T& x) {
            _root = insert_(_root, x);
        }

        bool empty() const {
            return (_root == nullptr);
        }

        void erase(const T& x){
            _root = erase_(_root, x);
        }

        size_t size() const {
            return size_(_root);
        }

    };

}
