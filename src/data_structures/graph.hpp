#pragma once
#include <vector>


namespace stlb
{

    template<class weight>
    struct edge_t
    {
        size_t x, y;
        weight w;

        bool operator< (const edge_t& cmp) const noexcept {
            return w < cmp.w;
        }
    };


    template<class weight>
    class undir_list_graph
    {

    private:

        std::vector<std::vector<std::pair<size_t, weight>>> _graph;
        size_t _vertices_count = 0;
        size_t _edges_count = 0;

    public:

        undir_list_graph(size_t N = 0) noexcept {
            resize(N);
        }

        void clear() noexcept {
            _graph.clear();
            _vertices_count = 0;
        }

        void resize(size_t size) noexcept {
            if (_vertices_count < size) {
                _vertices_count = size;
                _graph.resize(size);
            }
        }

        size_t size() const noexcept {
            return _vertices_count;
        }

        size_t edges_count() const noexcept {
            return _edges_count;
        }

        void add_edge(const size_t from, const size_t to, weight w) noexcept {
            resize(from > to ? from : to);
            _graph[from].push_back(std::make_pair(to, w));
            _graph[to].push_back(std::make_pair(from, w));
            _edges_count++;
        }

        std::vector<std::pair<size_t, weight>> get_next_vertices(const size_t vertex) const noexcept {
            return _graph[vertex];
        }

    };


    template<class weight>
    class dir_list_graph
    {

    private:

        std::vector<std::vector<std::pair<size_t, weight>>> _graph_up;
        std::vector<std::vector<std::pair<size_t, weight>>> _graph_down;
        size_t _vertices_count = 0;
        size_t _edges_count = 0;

    public:

        dir_list_graph(size_t N = 0) noexcept {
            resize(N);
        }

        void clear() noexcept {
            _graph_up.clear();
            _graph_down.clear();
            _vertices_count = 0;
        }

        void resize(size_t size) noexcept {
            if (_vertices_count < size) {
                _vertices_count = size;
                _graph_up.resize(size);
                _graph_down.resize(size);
            }
        }

        size_t size() const noexcept {
            return _vertices_count;
        }

        size_t edges_count() const noexcept {
            return _edges_count;
        }

        void add_edge(const size_t from, const size_t to, weight w) noexcept {
            resize(from > to ? from : to);
            _graph_up[from].push_back(std::make_pair(to, w));
            _graph_down[to].push_back(std::make_pair(from, w));
            _edges_count++;
        }

        std::vector<std::pair<size_t, weight>> get_next_vertices(const size_t vertex) const noexcept {
            return _graph_up[vertex];
        }

        std::vector<std::pair<size_t, weight>> get_prev_vertices(const size_t vertex) const noexcept {
            return _graph_down[vertex];
        }

    };

}
