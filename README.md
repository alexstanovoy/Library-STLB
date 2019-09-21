![Library::STLB](images/logo.svg)

## About [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) ![Build status](https://travis-ci.org/AjReme/Library-STLB.svg?branch=master) ##

***Library::STLB*** (*Standard Template Library B*) - the tiny library that contains implementations of powerful algorithms in **C++** language for different purposes: projects, as a small «*gear*» in big products, competitive programming solutions.  
Also, all algorithms and classes designed for intuitive use just like in **STD C++** library.  
- *All sources use only `.hpp` files so library using don't require any changes in your compilation line arguments*.
- *Docs* ***does not*** *contain detailed explanations, e.x. additional memory used, behavior in special cases... Sorry, I haven't got enough time for this*.
- *Library was tested manually, generating random tests, in different competitive programming tasks and even in contests*.
- *Some additional functionality for existing code, new algorithms and data structures will be added later*.

## Contents ##

- Data structures

	+ [Fibonacci heap](#Fibonacci-heap-(fib_heap.hpp))
	+ [Unsigned integer of fixed width](#Unsigned-integer-of-fixed-width-(fixed_uint.hpp))
	+ [Trie](#Trie-(trie.hpp))
	+ [Disjoint Set Union](#Disjoint-Set-Union-(dsu.hpp))
	+ [AVL tree set](#Set-(set.hpp))
	+ [Graphs](#Graphs-(graph.hpp))
	+ [Bit vector](#Bit-vector-(bitvec.hpp))

- Algorithms

	+ [Common algorithms](#Common-algorithms-(algorithm.hpp))
	+ [Huffman algorithm](#Huffman-algorithm-(huffman.hpp))
	+ [Minimal Spanning Tree](#Minimal-Spanning-Tree-(mst.hpp))
	+ [Shortest Path problem](#Shortest-Path-problem-(spp.hpp))


## Fibonacci heap (fib_heap.hpp) ##

Optimized fibonacci heap. *Note*: `push` operation returns `fib_heap<T>::pointer` that can be used in `decrease_key` function later. Requires `operator<`.

| **operation** | **complexity** | **explanation**                            |
|---------------|----------------|--------------------------------------------|
| size          | *O(1)*         | Returns size of container.                 |
| empty         | *O(1)*         | Tests whether container is empty.          |
| push          | *O(1)*         | Push new node to the heap.                 |
| top           | *O(1)*         | Returns value of the **smallest** element. |
| decrease_key  | *O(1)*         | Changes value of key to **smaller**.       |
| pop           | *O(logN)*      | Removes the top element.                   |
| clear         | *O(N)*         | Clears the container.                      |

## Unsigned integer of fixed width (fixed_uint.hpp) ##

Unsigned integer type designed to work like `uintXXX_t`, where `XXX` is any number of bits. Has overloaded input/output operators. ***Requires GNU GCC compiler***. May be considered as small «big integer». 
*I personally don't recommend to use this type for calculations larger than 512 bits*.  
**N** *hereinafter is width of type*.

| **operation**           | **complexity**                                   |
|-------------------------|--------------------------------------------------|
| Bitwise operators       | *O(N/64)*                                        |
| Increment and decrement | *O(N/64)*                                           |
| Comparision operators   | *O(N/64)*                                    |
| Sum                     | *O(N/64)*                                       |
| Subtraction             | *O(N/64)*                                       |
| Multiply                | *O(N²/64)*                                       |
| Division                | *O(N²/64)*                                       |
| to_string               | *O(N²/64 \* log<sub>10</sub>(2<sup>N</sup>)/19)* |
| operator<<              | *O(N²/64 \* log<sub>10</sub>(2<sup>N</sup>)/19)* |
| operator>>              | *O(N²/64 \* log<sub>10</sub>(2<sup>N</sup>)/19)* |

*Note*: implementation of multiplication will be rewrited in the near future.

## Trie (trie.hpp) ##

Trie-based structure. You should specify mapping value (as in `std::map`) and range of elements, example `trie<'a', 'z', int>`.  
**N** *is sum of lengths of inserted containers*.
**K** *is length of inserted container*.

`trie<min_val, max_val, value>`:

| **operation** | **complexity** | **explanation**                                                   |
|---------------|----------------|-------------------------------------------------------------------|
| size          | *O(1)*         | Returns size of container.                                        |
| empty         | *O(1)*         | Tests whether container is empty.                                 |
| begin         | *O(1)*         | Returns *iterator* to the root.                                   |
| end           | *O(1)*         | Returns *iterator* to the end of container.                       |
| insert        | *O(K)*         | Inserts container into trie and returns *iterator* to it.         |
| erase         | *O(K)*         | Erases **inserted** container from the trie.                      |
| operator[]    | *O(K)*         | Returns *value* of **inserted** container or inserts it before.   |
| find          | *O(K)*         | Returns *iterator* to **inserted** container or *end* iterator.   |
| find_prefix   | *O(K)*         | Returns *value* to **already inserted** prefix or *end* iterator. |
| clear         | *O(N)*         | Clears the trie.                                                  |

`trie<min_val, max_val, value>::iterator`:

| **operation** | **complexity** | **explanation**                                       |
|---------------|----------------|-------------------------------------------------------|
| operator[]    | *O(1)*         | Returns iterator to the next child or *end* iterator. |
| is_leaf       | *O(1)*         | Tests whether trie *have inserted* this node          |

# Disjoint Set Union (dsu.hpp)

Standard DSU.

| **operation**   | **complexity** | **explanation**                               |
|-----------------|----------------|-----------------------------------------------|
| leader          | *O(α(N))*      | Returns number of root node in the component. |
| united          | *O(α(N))*      | Tests whether two nodes are united.           |
| merge           | *O(α(N))*      | Merges two nodes.                             |
| resize          | *O(N)*         | Resizes DSU to the **bigger** size.           |
| clear           | *O(N)*         | Clears the container.                         |

## Set (set.hpp) ##

AVL tree set. Also, contains `order_of_key` and `key_by_order` order methods. Requires `operator<`. Numeration from zero.

| **operation** | **complexity** | **explanation**                             |
|---------------|----------------|---------------------------------------------|
| size          | *O(1)*         | Returns size of container.                  |
| empty         | *O(1)*         | Tests whether container is empty.           |
| insert        | *O(logN)*      | Inserts new **unique** element to the set.  |
| erase         | *O(logN)*      | Erases element by it's value.               |
| find          | *O(logN)*      | Test whether container contains element.    |
| order_of_key  | *O(logN)*      | Number of elements comparing less.          |
| key_by_order  | *O(logN)*      | Returns nth element in container.           |
| lower_bound   | *O(logN)*      | Returns first `>=` element in sorted order. |
| upper_bound   | *O(logN)*      | Returns first `>` element in sorted order.  |
| clear         | *O(N)*         | Clears the container.                       |

## Graphs (graph.hpp) ##

*Directed* and *undirected* list graphs are implemented here.  
**N** *is number of vertices*.
**K** *is length of returned container*.

| **operation**     | **complexity** | **explanation**                                          |
|-------------------|----------------|----------------------------------------------------------|
| add_edge          | *O(1)*         | Adds edge.                                               |
| size              | *O(1)*         | Returns size of container.                               |
| resize            | *O(1)*         | Resizes graph to the **bigger** size.                    |
| get_next_vertices | *O(K)*         | Returns *next* verticies.                                |
| get_prev_vertices | *O(K)*         | Returns *previous* verticies (*only for directed graph*) |
| clear             | *O(N)*         | Clears the graph from nodes.                             |

## Bit vector (bitvec.hpp) ##

This container provides methods for working with array as array of bits and bytes (that more useful than `std::bitset`). You can push bits, bytes, change bit or byte by index, concat bitvecs and so on.  
*Note*: Designed for Huffman encoding algorithm.

| **operation**       | **complexity** |
|---------------------|----------------|
| operator+(bitvec)   | *O(N+K)*       |
| operator+=(bitvec)  | *O(K)*         |
| operator+(uint8_t)  | *O(1)*         |
| operator+=(uint8_t) | *O(1)*         |
| push_bit            | *O(1)*         |
| pop_bit             | *O(1)*         |
| push_byte           | *O(1)*         |
| pop_byte            | *O(1)*         |
| byte_from_bit       | *O(1)*         |
| bit_size            | *O(1)*         |
| byte_size           | *O(1)*         |
| bit_at              | *O(1)*         |
| byte_at             | *O(1)*         |
| clear               | *O(N)*         |

## Common algorithms (algorithm.hpp) ##

- `radix_sort` (*radix sort*) - very fast sort of unsigned integer container. Complexity: *O(NK)*, where **K** is `sizeof(key)`.
- `radix_nth_element` (*radix selest*) - very fast function that returns nth-element in container. Complexity: *O(NK)*, where **K** is `sizeof(key)`.
- `prefix_function` (*prefix function*) - implementation of prefix function algorithm. Complexity: *O(N)*.
- `z_function` (*z-function*) - implementation of z-function algorithm. Complexity: *O(N)*.
- `manacher` - implementation of manacher algorithm. Complexity: *O(N)*.

*Notes*:
- `radix_sort` and `radix_nth_element` are both *stable*.
- Key getter can be provided. That means you can sort not only integer containers. Key must be fixed-length integer (`__int128` *also can be a key*).
- `radix_sort` works about **3 times** faster than `boost::sort::spreadsort::integer_sort`, guarantees same work time for containers with same length, type and key width. Also it is **more than 7 times** faster than `std::sort`.
- This functions work with forward iterator (`std::list` and `std::forward_list` can be sorted).

## Huffman algorithm (huffman.hpp) ##

Efficient static Huffman algorithm for file compression. `bitvec` needed as input. Complexity: *O(NlogN)*, where **N** is number of bytes in file.

*Winner of MIPT DIHT 2019 Huffman competition*.

## Minimal Spanning Tree (mst.hpp) ##

Here are implemented some MST algorithms.

- `kruskal`: Kruskal algorithm. Complexity: *O(E(logE+α(V)))*.
- `boruvka`: Borůvka's algorithm. Complexity: *O(ElogV)*.
- `prim_bin_heap`: Prim algorithm based on binary queue. Complexity: *O(E+Vlog(E/V)logV)*.
- `prim_fib_heap`: Prim algorithm based on fibonacci heap. Complexity: *O(E+VlogV)*.

## Shortest Path problem (spp.hpp) ##

Here are implemented some shortest path algorithms.

- `dijkstra_bin_heap`: Dijkstra algorithm based on binary queue. Complexity: *O(E+Vlog(E/V)logV)*.
- `dijkstra_bin_tree`: Dijkstra algorithm based on red-black tree. Complexity: *O((E+V)logV)*.
- `dijkstra_fib_heap`: Dijkstra algorithm based on fibonacci queue. Complexity: *O(E+VlogV)*.
- `ford_bellman`: Ford-Bellman algorithm. Complexity: *O(VE)*.
- `SPFA` (*Shortest Path Faster Algorithm*): Improved Ford-Bellman algorithm, that runs much faster on average. Worst-case complexity: *O(VE)*. Average Complexity: *O(ElogE)*.

