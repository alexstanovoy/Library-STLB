#pragma once
#include <queue>
#include <vector>
#include <climits>
#include <algorithm>
#include <functional>
#include "graph.hpp"


namespace stlb
{

    template<typename _weight>
    class __dinitz
    {

    private:

        const size_t _src;
        const size_t _dst;
        network<_weight>& _net;
        std::vector<size_t> _layer;
        std::vector<typename network<_weight>::iterator> _ptr;

        bool construct_layers_() noexcept {
            std::fill(_layer.begin(), _layer.end(), std::numeric_limits<size_t>::max());
            std::queue<size_t> q;
            _layer[_src] = 0;
            q.push(_src);
            while (!q.empty() && _layer[_dst] == std::numeric_limits<size_t>::max()) {
                const size_t v = q.front(); q.pop();
                std::for_each(_net.begin(v), _net.end(v), [&](const typename network<_weight>::flow_edge_t& edge) noexcept {
                    if (!edge.is_full() && _layer[edge.to] == ~static_cast<size_t>(0)) {
                        _layer[edge.to] = _layer[v] + 1;
                        q.push(edge.to);
                    }
                });
            }
            return _layer[_dst] != std::numeric_limits<size_t>::max();
        }

        _weight push_(
            const size_t v,
            const _weight flow = std::numeric_limits<_weight>::max()
        ) noexcept {
            if (v == _dst || flow == static_cast<_weight>(0)) {
                return flow;
            }
            for (; _ptr[v] != _net.end(v); ++_ptr[v]) {
                const size_t u = (*_ptr[v]).to;
                if (_layer[u] == _layer[v] + 1) {
                    const _weight free = (*_ptr[v]).free();
                    const _weight pushed = push_(u, std::min(flow, free));
                    if (pushed > static_cast<_weight>(0)) {
                        _net.push(pushed, _ptr[v]);
                        return pushed;
                    }
                }
            }
            return static_cast<_weight>(0);
        }

    public:

        __dinitz() = delete;

        __dinitz(
            network<_weight>& net,
            const size_t source,
            const size_t destination
        ) noexcept
            : _src(source), _dst(destination), _net(net),
              _layer(net.size()), _ptr(net.size()) {}

        ~__dinitz() noexcept = default;

        _weight flow() noexcept {
            _weight ret = static_cast<_weight>(0);
            while (construct_layers_()) {
                for (size_t i = 0; i < _net.size(); ++i) {
                    _ptr[i] = _net.begin(i);
                }
                _weight pushed = push_(_src);
                while (pushed > 0) {
                    ret += pushed;
                    pushed = push_(_src);
                }
            }
            return ret;
        }

    };

    template<typename _weight>
    _weight dinitz(
        network<_weight>& net,
        const size_t source,
        const size_t destination
    ) noexcept {
        return __dinitz<_weight>(net, source, destination).flow();
    }

    template<typename _weight>
    class __push_relabel
    {

    private:

        const size_t _src;
        const size_t _dst;
        std::vector<size_t> _height;
        std::vector<_weight> _excess;
        network<_weight>& _net;

        void push_(const size_t v, typename network<_weight>::iterator& it) noexcept {
            const _weight lim = std::min(_excess[v], (*it).free());
            _net.push(lim, it);
            _excess[v] -= lim;
            _excess[(*it).to] += lim;
        }

        void relabel_(const size_t v) noexcept {
            size_t min_height = std::numeric_limits<size_t>::max();
            std::for_each(_net.begin(v), _net.end(v), [&](const typename network<_weight>::flow_edge_t& edge) noexcept {
                if (!edge.is_full()) {
                    min_height = std::min(min_height, _height[edge.to]);
                }
            });
            if (min_height != std::numeric_limits<size_t>::max()) {
                _height[v] = min_height + 1;
            }
        }

        std::vector<size_t> max_height_vertices_() noexcept {
            size_t max_height = 0;
            size_t cnt = 0;
            for (size_t i = 0; i < _net.size(); ++i) {
                if (i != _src && i != _dst && _excess[i] > static_cast<_weight>(0)) {
                    if (max_height < _height[i]) {
                        max_height = _height[i];
                        cnt = 1;
                    }
                    else if (max_height == _height[i]) {
                        ++cnt;
                    }
                }
            }
            std::vector<size_t> ret;
            ret.reserve(cnt);
            for (size_t i = 0; i < _net.size(); ++i) {
                if (i != _src && i != _dst && _excess[i] > static_cast<_weight>(0) && _height[i] == max_height) {
                    ret.push_back(i);
                }
            }
            return ret;
        }

    public:

        __push_relabel(
            network<_weight>& net,
            const size_t source,
            const size_t destination
        ) noexcept
            : _src(source), _dst(destination), _height(net.size(), 0),
              _excess(net.size(), static_cast<_weight>(0)), _net(net) {}

        __push_relabel() = delete;

        ~__push_relabel() noexcept = default;

        _weight flow() noexcept {
            _height[_src] = _net.size();
            _excess[_src] = std::numeric_limits<_weight>::max();
            for (typename network<_weight>::iterator it = _net.begin(_src); it != _net.end(_src); ++it) {
                push_(_src, it);
            }
            std::vector<size_t> cur;
            while (!(cur = max_height_vertices_()).empty()) {
                for (const size_t v : cur) {
                    bool pushed = false;
                    for (typename network<_weight>::iterator it = _net.begin(v); it != _net.end(v); ++it) {
                        if (_excess[v] == static_cast<_weight>(0)) {
                            break;
                        }
                        if (!(*it).is_full() && _height[v] == _height[(*it).to] + 1) {
                            push_(v, it);
                            pushed = true;
                        }
                    }
                    if (!pushed) {
                        relabel_(v);
                        break;
                    }
                }
            }
            _weight ret = static_cast<_weight>(0);
            std::for_each(_net.begin(_src), _net.end(_src), [&](const typename network<_weight>::flow_edge_t& edge) noexcept {
                ret += edge.flow;
            });
            return ret;
        }

    };

    template<typename _weight>
    _weight push_relabel(
        network<_weight> net,
        const size_t source,
        const size_t destination
    ) noexcept {
        return __push_relabel<_weight>(net, source, destination).flow();
    } // Check get_cut

    template<typename _weight>
    class __malhotra
    {

    private:

        static const size_t dist_lim = std::numeric_limits<size_t>::max() - 1;
        const size_t _src;
        const size_t _dst;
        std::vector<size_t> _dist;
        std::vector<_weight> _to_push;
        std::vector<_weight> _pot_in;
        std::vector<_weight> _pot_out;
        network<_weight>& _net;

        void init_potentials_() noexcept {
            std::fill(_pot_in.begin(), _pot_in.end(), static_cast<_weight>(0));
            std::fill(_pot_out.begin(), _pot_out.end(), static_cast<_weight>(0));
            for (size_t i = 0; i < _net.size(); ++i) {
                std::for_each(_net.begin(i), _net.end(i), [&](const typename network<_weight>::flow_edge_t& edge) noexcept {
                    if (_dist[edge.to] == _dist[i] + 1) {
                        const _weight pot = edge.free();
                        _pot_in[edge.to] += pot;
                        _pot_out[i] += pot;
                    }
                });
            }
        }

        _weight get_pot_(const size_t v) const noexcept {
            if (v == _src) {
                return _pot_out[v];
            }
            if (v == _dst) {
                return _pot_in[v];
            }
            return std::min(_pot_in[v], _pot_out[v]);
        }

        bool construct_dist_() noexcept {
            std::queue<size_t> q;
            std::fill(_dist.begin(), _dist.end(), dist_lim);
            _dist[_src] = 0;
            q.push(_src);
            while (!q.empty()) {
                const size_t v = q.front(); q.pop();
                std::for_each(_net.begin(v), _net.end(v), [&](const typename network<_weight>::flow_edge_t& edge) {
                    if (!edge.is_full() && _dist[edge.to] == dist_lim) {
                        _dist[edge.to] = _dist[v] + 1;
                        q.push(edge.to);
                    }
                });
            }
            return _dist[_dst] != dist_lim;
        }

        void push_(const size_t start, const _weight weight, const bool dir) noexcept {
            std::queue<size_t> q;
            std::fill(_to_push.begin(), _to_push.end(), 0);
            _to_push[start] += weight;
            q.push(start);
            while (!q.empty()) {
                const size_t v = q.front(); q.pop();
                const typename network<_weight>::iterator beg = (dir ? _net.begin(v) : _net.rbegin(v));
                const typename network<_weight>::iterator end = (dir ? _net.end(v) : _net.rend(v));
                for (typename network<_weight>::iterator it = beg; it != end; ++it) {
                    if (_to_push[v] == static_cast<_weight>(0)) {
                        break;
                    }
                    const size_t u = (dir ? (*it).to : (*it).from);
                    if (dir) {
                        if (_dist[u] != _dist[v] + 1) {
                            continue;
                        }
                    }
                    else {
                        if (_dist[v] != _dist[u] + 1) {
                            continue;
                        }
                    }
                    const _weight pushed = std::min((*it).free(), _to_push[v]);
                    if (pushed == static_cast<_weight>(0)) {
                        continue;
                    }
                    if (_to_push[u] == static_cast<_weight>(0) && u != _dst) {
                        q.push(u);
                    }
                    _net.push(pushed, it);
                    if (dir) {
                        _pot_in[u] -= pushed;
                        _pot_out[v] -= pushed;
                    }
                    else {
                        _pot_out[u] -= pushed;
                        _pot_in[v] -= pushed;
                    }
                    _to_push[v] -= pushed;
                    _to_push[u] += pushed;
                };
            }
        }

        _weight blocking_flow_() noexcept {
            while (true) {
                size_t min_pot_ver = std::numeric_limits<size_t>::max();
                for (size_t i = 0; i < _net.size(); ++i) {
                    if (_dist[i] == dist_lim) {
                        continue;
                    }
                    if (min_pot_ver == std::numeric_limits<size_t>::max() || (get_pot_(i) < get_pot_(min_pot_ver))) {
                        min_pot_ver = i;
                    }
                }
                if (min_pot_ver == std::numeric_limits<size_t>::max()) {
                    return static_cast<_weight>(0);
                }
                if (get_pot_(min_pot_ver) == static_cast<_weight>(0)) {
                    std::for_each(_net.begin(min_pot_ver), _net.end(min_pot_ver), [&](const typename network<_weight>::flow_edge_t& edge) {
                        if (_dist[edge.to] == _dist[min_pot_ver] + 1) {
                            const _weight pushed = edge.free();
                            _pot_out[min_pot_ver] -= pushed;
                            _pot_in[edge.to] -= pushed;
                        }
                    });
                    std::for_each(_net.rbegin(min_pot_ver), _net.rend(min_pot_ver), [&](const typename network<_weight>::flow_edge_t& edge) {
                        if (_dist[min_pot_ver] == _dist[edge.from] + 1) {
                            const _weight pushed = edge.free();
                            _pot_in[min_pot_ver] -= pushed;
                            _pot_out[edge.from] -= pushed;
                        }
                    });
                    _dist[min_pot_ver] = dist_lim;
                    continue;
                }
                const _weight ret = get_pot_(min_pot_ver);
                push_(min_pot_ver, ret, true);
                push_(min_pot_ver, ret, false);
                return ret;
            }
        }

    public:

        __malhotra(
            network<_weight>& net,
            const size_t source,
            const size_t destination
        ) noexcept
            : _src(source), _dst(destination), _dist(net.size()),
              _to_push(net.size()), _pot_in(net.size()), _pot_out(net.size()), _net(net) {}

        _weight flow() noexcept {
            _weight ret = static_cast<_weight>(0);
            while (construct_dist_()) {
                init_potentials_();
                _weight pushed = blocking_flow_();
                while (pushed > static_cast<_weight>(0)) {
                    ret += pushed;
                    pushed = blocking_flow_();
                }
            }
            return ret;
        }

    };

    template<typename _weight>
    _weight malhotra(
        network<_weight> net,
        const size_t source,
        const size_t destination
    ) noexcept {
        return __malhotra<_weight>(net, source, destination).flow();
    } // Check get_cut

}
