#pragma once
#include <vector>
#include <cstddef>
#include <iterator>
#include <algorithm>
#include <functional>


namespace stlb
{

    template<typename _weight>
    struct edge_t
    {
        size_t x;
        size_t y;
        _weight weight;

        constexpr edge_t(const size_t x, const size_t y, const _weight weight) noexcept : x(x), y(y), weight(weight) {}

        bool operator< (const edge_t& cmp) const noexcept {
            return weight < cmp.weight;
        }
    };

    template<class _weight>
    class graph
    {

    public:

        virtual ~graph();

        virtual size_t size();

        virtual size_t edges_count();

        virtual void add_edge(const size_t from, const size_t to, const _weight weight) const;

        virtual std::vector<std::pair<size_t, _weight>> get_next_vertices(const size_t vertex) const;

        virtual std::vector<std::pair<size_t, _weight>> get_prev_vertices(const size_t vertex) const;

    };

    template<typename _weight>
    class list_graph : public graph<_weight>
    {

    private:

        size_t _vertices_count = 0;
        size_t _edges_count = 0;
        std::vector<std::vector<std::pair<size_t, _weight>>> _graph_up;
        std::vector<std::vector<std::pair<size_t, _weight>>> _graph_down;

    public:

        constexpr list_graph(const size_t N = 0) noexcept
            : _vertices_count(N), _graph_up(N), _graph_down(N) {}

        constexpr list_graph(const list_graph<_weight>& other) noexcept = default;

        ~list_graph() noexcept = default;

        list_graph<_weight>& operator= (const list_graph<_weight>& other) noexcept = default;

        size_t size() const noexcept {
            return _vertices_count;
        }

        size_t edges_count() const noexcept {
            return _edges_count;
        }

        list_graph<_weight>& add_edge(const size_t from, const size_t to, const _weight weight) noexcept {
            _graph_up[from].push_back(std::make_pair(to, weight));
            _graph_down[to].push_back(std::make_pair(from, weight));
            ++_edges_count;
            return *this;
        }

        std::vector<std::pair<size_t, _weight>> get_next_vertices(const size_t vertex) const noexcept {
            return _graph_up[vertex];
        }

        std::vector<std::pair<size_t, _weight>> get_prev_vertices(const size_t vertex) const noexcept {
            return _graph_down[vertex];
        }

    };

    template<typename _weight>
    class network
    {

    public:

        struct flow_edge_t
        {

            const size_t from;
            const size_t to;
            _weight cap;
            _weight flow;

            flow_edge_t() noexcept = default;

            flow_edge_t(
                const size_t from,
                const size_t to,
                const _weight cap,
                const _weight flow
            ) noexcept : from(from), to(to), cap(cap), flow(flow) {}

            ~flow_edge_t() noexcept = default;

            flow_edge_t& operator= (const flow_edge_t& other) noexcept = default;

            _weight free() const noexcept {
                return cap - flow;
            }

            _weight is_full() const noexcept {
                return free() <= static_cast<_weight>(0);
            }

        };

    private:

        friend class iterator;

        const size_t _vertices_count;
        std::vector<std::vector<size_t>> _num;
        std::vector<std::vector<size_t>> _rev_num;
        std::vector<flow_edge_t> _edges;

    public:

        class iterator : public std::iterator<
            std::random_access_iterator_tag,
            flow_edge_t
        > {

        private:

            friend class network;

            network<_weight>* _net;
            typename std::vector<size_t>::iterator _ptr;

            iterator(
                network<_weight> *const net,
                const typename std::vector<size_t>::iterator& it
            ) noexcept : _net(net), _ptr(it) {}

        public:

            iterator() noexcept = default;

            iterator(const iterator& other) noexcept = default;

            ~iterator() noexcept = default;

            iterator& operator= (const iterator& other) noexcept = default;

            bool operator== (const iterator& other) const noexcept {
                return _ptr == other._ptr;
            }

            bool operator!= (const iterator& other) const noexcept {
                return _ptr != other._ptr;
            }

            flow_edge_t& operator* () noexcept {
                return _net->_edges[*_ptr];
            }

            flow_edge_t operator* () const noexcept {
                return _net._edges[*_ptr];
            }

            iterator& operator++ () noexcept {
                ++_ptr;
                return *this;
            }

            iterator operator++ (int) noexcept {
                iterator cpy = *this;
                ++*this;
                return cpy;
            }

            iterator& operator-- () noexcept {
                --_ptr;
                return *this;
            }

            iterator operator-- (int) noexcept {
                iterator cpy = *this;
                --*this;
                return cpy;
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

            flow_edge_t& operator[] (const ptrdiff_t ind) noexcept {
                return *(_ptr + ind);
            }

            flow_edge_t operator[] (const ptrdiff_t ind) const noexcept {
                return *(_ptr + ind);
            }

        };

        network(const size_t N) noexcept : _vertices_count(N), _num(N), _rev_num(N) {}

        size_t size() const noexcept {
            return _vertices_count;
        }

        network<_weight>& add_edge(const size_t from, const size_t to, const _weight cap) noexcept {
            _num[from].push_back(_edges.size());
            _rev_num[to].push_back(_edges.size());
            _edges.emplace_back(from, to, cap, static_cast<_weight>(0));
            _num[to].push_back(_edges.size());
            _rev_num[from].push_back(_edges.size());
            _edges.emplace_back(to, from, static_cast<_weight>(0), static_cast<_weight>(0));
            return *this;
        }

        network<_weight>& push(const _weight weight, const iterator& it) noexcept {
            const size_t id = *it._ptr;
            _edges[id].flow += weight;
            _edges[id ^ 1].flow -= weight;
            return *this;
        }

        iterator begin(const size_t vertex) noexcept {
            return iterator(this, _num[vertex].begin());
        }

        iterator end(const size_t vertex) noexcept {
            return iterator(this, _num[vertex].end());
        }

        iterator rbegin(const size_t vertex) noexcept {
            return iterator(this, _rev_num[vertex].begin());
        }

        iterator rend(const size_t vertex) noexcept {
            return iterator(this, _rev_num[vertex].end());
        }

        std::vector<bool> get_cut(
            const size_t source
        ) noexcept {
            std::vector<bool> ret(_vertices_count, false);
            std::function<void(const size_t)> dfs = [&](const size_t v) noexcept {
                if (!ret[v]) {
                    ret[v] = true;
                    std::for_each(begin(v), end(v), [&](const flow_edge_t& edge) noexcept {
                        if (!edge.is_full()) {
                            dfs(edge.to);
                        }
                    });
                }
            };
            dfs(source);
            return ret;
        }

        const std::vector<flow_edge_t>& get_edges() const noexcept {
            return _edges;
        }

        network<_weight>& clear() noexcept {
            std::for_each(_edges.begin(), _edges.end(), [](flow_edge_t& edge) noexcept {
                edge.flow = static_cast<_weight>(0);
            });
            return *this;
        }

    };

}
