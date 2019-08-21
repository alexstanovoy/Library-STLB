#ifndef __STLB_SPP_ALGO
#define __STLB_SPP_ALGO 1

#include "graph.hpp"
#include "fib_heap.hpp"
#include "set.hpp"
#include <set>
#include <queue>
#include <vector>


namespace stlb
{

    template<class weight>
    auto dijkstra_bin_heap(const undir_list_graph<weight>& graph, const size_t start) {
        std::vector<std::pair<weight, size_t>> dist(
            graph.size(),
            std::make_pair(weight(), -1)
        );

        std::vector<bool> used(graph.size(), false);
        std::vector<bool> inf(graph.size(), true);
        std::priority_queue<
            std::pair<weight, size_t>,
            std::vector<std::pair<weight, size_t>>,
            std::greater<std::pair<weight, size_t>>
        > q;
        inf[start] = false;
        q.push(std::make_pair(weight(), start));

        while (!q.empty()) {
            size_t v = q.top().second; q.pop();
            if (!used[v]) {
                used[v] = true;
                for (const auto& u : graph.get_next_vertices(v)) {
                    if (inf[u.first] || dist[v].first + u.second < dist[u.first].first) {
                        inf[u.first] = false;
                        dist[u.first] = std::make_pair(dist[v].first + u.second, v);
                        q.push(std::make_pair(dist[u.first].first, u.first));
                    }
                }
            }
        }

        return dist;
    }


    template<class weight>
    auto dijkstra_rb_tree(const undir_list_graph<weight>& graph, const size_t start) {
        std::vector<std::pair<weight, size_t>> dist(
            graph.size(),
            std::make_pair(weight(), -1)
        );

        std::vector<bool> inf(graph.size(), true);
        std::set<std::pair<weight, size_t>> q;
        inf[start] = false;
        q.insert(std::make_pair(weight(), start));

        while (!q.empty()) {
            size_t v = q.begin()->second; q.erase(q.begin());
            for (const auto& u : graph.get_next_vertices(v)) {
                if (inf[u.first]) {
                    inf[u.first] = false;
                    dist[u.first] = std::make_pair(dist[v].first + u.second, v);
                    q.insert(std::make_pair(dist[u.first].first, u.first));
                }
                else if (dist[v].first + u.second < dist[u.first].first) {
                    q.erase(std::make_pair(dist[u.first].first, u.first));
                    dist[u.first] = std::make_pair(dist[v].first + u.second, v);
                    q.insert(std::make_pair(dist[u.first].first, u.first));
                }
            }
        }

        return dist;
    }


    template<class weight>
    auto dijkstra_avl_tree(const undir_list_graph<weight>& graph, const size_t start) {
        std::vector<std::pair<weight, size_t>> dist(
            graph.size(),
            std::make_pair(weight(), -1)
        );

        std::vector<bool> inf(graph.size(), true);
        stlb::set<std::pair<weight, size_t>> q;
        inf[start] = false;
        q.insert(std::make_pair(weight(), start));

        while (!q.empty()) {
            size_t v = q.find_by_order(0).second; q.erase(q.find_by_order(0));
            for (const auto& u : graph.get_next_vertices(v)) {
                if (inf[u.first]) {
                    inf[u.first] = false;
                    dist[u.first] = std::make_pair(dist[v].first + u.second, v);
                    q.insert(std::make_pair(dist[u.first].first, u.first));
                }
                else if (dist[v].first + u.second < dist[u.first].first) {
                    q.erase(std::make_pair(dist[u.first].first, u.first));
                    dist[u.first] = std::make_pair(dist[v].first + u.second, v);
                    q.insert(std::make_pair(dist[u.first].first, u.first));
                }
            }
        }

        return dist;
    }


    template<class weight>
    auto dijkstra_fib_heap(const undir_list_graph<weight>& graph, const size_t start) {
        std::vector<std::pair<weight, size_t>> dist(
            graph.size(),
            std::make_pair(weight(), -1)
        );

        std::vector<bool> inf(graph.size(), true);
        std::vector<
            typename fib_heap<std::pair<weight, size_t>
        >::pointer> ptrs(graph.size());
        fib_heap<std::pair<weight, size_t>> q;
        inf[start] = false;
        ptrs[start] = q.push(std::make_pair(weight(), start));

        while (!q.empty()) {
            size_t v = q.top().second; q.pop();
            for (const auto& u : graph.get_next_vertices(v)) {
                if (inf[u.first]) {
                    inf[u.first] = false;
                    dist[u.first] = std::make_pair(dist[v].first + u.second, v);
                    ptrs[u.first] = q.push(std::make_pair(dist[u.first].first, u.first));
                }
                else if (dist[v].first + u.second < dist[u.first].first) {
                    dist[u.first] = std::make_pair(dist[v].first + u.second, v);
                    q.decrease_key(ptrs[u.first], std::make_pair(dist[u.first].first, u.first));
                }
            }
        }

        return dist;
    }


    template<class weight>
    auto ford_bellman(const undir_list_graph<weight>& graph, const size_t start) {
        std::vector<edge_t<weight>> edges;
        for (size_t i = 0; i < graph.size(); i++) {
            for (const auto& j : graph.get_next_vertices(i)) {
                edges.push_back({ i, j.first, j.second });
            }
        }

        std::vector<bool> inf(graph.size(), true);
        std::vector<std::pair<weight, size_t>> dist(
            graph.size(),
            std::make_pair(weight(), -1)
        );
        inf[start] = false;

        while (true) {
            bool updated = false;
            for (const auto& e : edges) {
                if (!inf[e.x]) {
                    if (inf[e.y] || dist[e.x].first + e.w < dist[e.y].first) {
                        inf[e.y] = false;
                        dist[e.y] = std::make_pair(dist[e.x].first + e.w, e.x);
                        updated = true;
                    }
                }
            }
            if (!updated) {
                break;
            }
        }

        return dist;
    }


    template<class weight>
    auto SPFA(const undir_list_graph<weight>& graph, const size_t start) {
        std::vector<bool> in_queue(graph.size(), false);
        std::vector<bool> inf(graph.size(), true);
        std::vector<std::pair<weight, size_t>> dist(
            graph.size(),
            std::make_pair(weight(), -1)
        );
        std::queue<size_t> q;

        q.push(start);
        inf[start] = false;
        in_queue[start] = true;
        while (!q.empty()) {
            size_t v = q.front(); q.pop();
            in_queue[v] = false;
            for (const auto& u : graph.get_next_vertices(v)) {
                if (inf[u.first] || dist[v].first + u.second < dist[u.first].first) {
                    inf[u.first] = false;
                    dist[u.first] = std::make_pair(dist[v].first + u.second, v);
                    if (!in_queue[u.first]) {
                        in_queue[u.first] = true;
                        q.push(u.first);
                    }
                }
            }
        }

        return dist;
    }

}

#endif //  __STLB_SPP_ALGO
