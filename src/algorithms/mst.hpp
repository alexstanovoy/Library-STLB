#pragma once
#include <queue>
#include <vector>
#include "dsu.hpp"
#include "graph.hpp"
#include "fib_heap.hpp"
#include "algorithm.hpp"


namespace stlb
{

    template<class weight>
    auto kruskal_comp(const undir_list_graph<weight>& graph) {
        std::vector<edge_t<weight>> edges, ret;
        edges.reserve(graph.edges_count());
        ret.reserve(graph.size() - 1);
        for (size_t i = 0; i < graph.size(); i++) {
            for (const auto& j : graph.get_next_vertices(i)) {
                edges.push_back({ i, j.first, j.second });
            }
        }

        dsu comp(graph.size());
        std::sort(edges.begin(), edges.end());
        for (const auto& i : edges) {
            if (!comp.united(i.x, i.y)) {
                comp.merge(i.x, i.y);
                ret.push_back(i);
            }
        }

        return ret;
    }


    template<class weight>
    auto kruskal_radix(const undir_list_graph<weight>& graph) {
        std::vector<edge_t<weight>> edges, ret;
        edges.reserve(graph.edges_count());
        ret.reserve(graph.size() - 1);
        for (size_t i = 0; i < graph.size(); i++) {
            for (const auto& j : graph.get_next_vertices(i)) {
                edges.push_back({ i, j.first, j.second });
            }
        }

        dsu comp(graph.size());
        stlb::radix_sort(edges.begin(), edges.end(), [](const edge_t<weight>& x){ return x.w; });
        for (const auto& i : edges) {
            if (!comp.united(i.x, i.y)) {
                comp.merge(i.x, i.y);
                ret.push_back(i);
            }
        }

        return ret;
    }


    template<class weight>
    auto boruvka(const undir_list_graph<weight>& graph) {
        std::vector<edge_t<weight>> edges, ret;
        dsu comp(graph.size());
        edges.reserve(graph.edges_count());
        ret.reserve(graph.size() - 1);

        for (size_t i = 0; i < graph.size(); i++) {
            for (const auto& j : graph.get_next_vertices(i)) {
                edges.push_back({ i, j.first, j.second });
            }
        }

        while (ret.size() < graph.size() - 1) {
            std::vector<bool> founded(graph.size(), false);
            std::vector<std::pair<weight, size_t>> add_edge(
                graph.size(),
                std::make_pair(weight(), 0)
            );
            for (const auto& i : edges) {
                if (!comp.united(i.x, i.y)) {
                    size_t x_num = comp.leader(i.x);
                    size_t y_num = comp.leader(i.y);
                    if (!founded[x_num] || std::make_pair(i.w, y_num) < add_edge[x_num]) {
                        founded[x_num] = true;
                        add_edge[x_num] = std::make_pair(i.w, y_num);
                    }
                    if (!founded[y_num] || std::make_pair(i.w, x_num) < add_edge[y_num]) {
                        founded[y_num] = true;
                        add_edge[y_num] = std::make_pair(i.w, x_num);
                    }
                }
            }
            size_t prev_size = ret.size();
            for (size_t i = 0; i < add_edge.size(); i++) {
                if (founded[i]) {
                    if (i != add_edge[add_edge[i].second].second || i < add_edge[i].second) {
                        comp.merge(i, add_edge[i].second);
                        ret.push_back({ i, add_edge[i].second, add_edge[i].first });
                    }
                }
            }
            if (prev_size == ret.size()) {
                break;
            }
        }

        return ret;
    }


    template<class weight>
    auto prim_bin_heap(const undir_list_graph<weight>& graph) {
        std::vector<edge_t<weight>> ret;
        ret.reserve(graph.size() - 1);

        std::vector<bool> inf(graph.size(), true);
        std::vector<bool> in_mst(graph.size(), false);
        std::vector<std::pair<size_t, weight>> edges(graph.size());
        std::priority_queue<
            std::pair<weight, size_t>,
            std::vector<std::pair<weight, size_t>>,
            std::greater<std::pair<weight, size_t>>
        > q;
        inf[0] = false;
        q.push(std::make_pair(weight(), 0));

        while (!q.empty()) {
            size_t v = q.top().second; q.pop();
            if (!in_mst[v]) {
                in_mst[v] = true;
                if (v != 0) {
                    ret.push_back({ v, edges[v].first, edges[v].second });
                }
                for (const auto& u : graph.get_next_vertices(v)) {
                    if (inf[u.first] || !in_mst[u.first] && u.second < edges[u.first].second) {
                        inf[u.first] = false;
                        edges[u.first] = std::make_pair(v, u.second);
                        q.push(std::make_pair(u.second, u.first));
                    }
                }
            }
        }

        return ret;
    }


    template<class weight>
    auto prim_fib_heap(const undir_list_graph<weight>& graph) {
        std::vector<edge_t<weight>> ret;
        ret.reserve(graph.size() - 1);

        std::vector<bool> inf(graph.size(), true);
        std::vector<bool> in_mst(graph.size(), false);
        std::vector<std::pair<size_t, weight>> edges(graph.size());
        std::vector<
            typename fib_heap<std::pair<weight, size_t>
        >::pointer> ptrs(graph.size());
        fib_heap<std::pair<weight, size_t>> q;
        inf[0] = false;
        ptrs[0] = q.push(std::make_pair(weight(), 0));

        while (!q.empty()) {
            size_t v = q.top().second; q.pop();
            if (v != 0) {
                ret.push_back({ v, edges[v].first, edges[v].second });
            }
            in_mst[v] = true;
            for (const auto& u : graph.get_next_vertices(v)) {
                if (inf[u.first]) {
                    inf[u.first] = false;
                    edges[u.first] = std::make_pair(v, u.second);
                    ptrs[u.first] = q.push(std::make_pair(u.second, u.first));
                }
                else if (!in_mst[u.first] && u.second < edges[u.first].second) {
                    edges[u.first] = std::make_pair(v, u.second);
                    q.decrease_key(ptrs[u.first], std::make_pair(u.second, u.first));
                }
            }
        }

        return ret;
    }

}
