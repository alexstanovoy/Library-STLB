#pragma once
#include <vector>


namespace stlb
{

    class dsu
    {

    private:

        std::vector<size_t> _parent;
        std::vector<size_t> _rank;
        size_t _vertices_count = 0;

    public:

        dsu(const size_t size) noexcept {
            resize(size);
        }

        void resize(const size_t size) noexcept {
            if (_vertices_count < size) {
                _parent.resize(size);
                _rank.resize(size);
                for (; _vertices_count < size; _vertices_count++) {
                    _parent[_vertices_count] = _vertices_count;
                    _rank[_vertices_count] = 0;
                }
            }
        }

        void clear() noexcept {
            _vertices_count = 0;
            _parent.clear();
            _rank.clear();
        }

        size_t size() const noexcept {
            return _vertices_count;
        }

        size_t leader(const size_t x) noexcept {
            return _parent[x] == x ? x : _parent[x] = leader(_parent[x]);
        }

        bool united(const size_t x, const size_t y) noexcept {
            return leader(x) == leader(y);
        }

        void merge(size_t x, size_t y) noexcept {
            x = leader(x);
            y = leader(y);
            if (x != y) {
                if (_rank[x] < _rank[y]) {
                    std::swap(x, y);
                }
                _parent[y] = x;
                if (_rank[x] == _rank[y]) {
                    _rank[x]++;
                }
            }
        }

    };

}
