#pragma once

#include <memory>
#include <utility>
#include <list>
#include <vector>
#include <set>

// TODO allow usage of vectors instead of a map if the ids of the vertices are
// numbers

namespace domidimi {


// represent nodes as keys in a map
// if an edge exist between node and it's values
template <
    // looks like std::set outperforms unordered_set for the find which is the
    // most used call
    typename MapType
>
class Topsort {
public:

    typedef typename MapType::key_type Node;

    /**
     * @brief
     * @param graph
     *
     * example:
     * std::unordered_map<int, std::set<int>> m = {{3,{1}}, {2,{1}}};
     * graph:
     * first -> then
     * 1 -> 3, 1 -> 2
     * or
     * 3 depends on 1 and 2 depends on 1
     */
    explicit Topsort(const MapType & graph) : graph_(graph) {
        initialize();
    }

    explicit Topsort(MapType && graph) : graph_(std::move(graph)) {
        initialize();
    }

    /*
    template<typename ItrType>
        typename std::iterator_traits<ItrType>::value_type::first_type &
        getKey(ItrType &itr, std::size_t idx) {
            return itr->first;
        };
    */

    /**
     * @brief Get the next Node according to the topological sort
     * Warning: This function does not check if there are still nodes, so the
     * developer has to call has_next() on his own
     *
     * @return the next node
     */
    Node next() {
        auto sorted_elem = S.front();

        for (auto it_graph = graph_.begin(); it_graph != graph_.end(); ) {
            // auto &key = getKey(it_graph, idx);
            auto &key = it_graph->first;
            auto &lst = it_graph->second;
            auto elem_it = lst.find(sorted_elem);

            if (elem_it != lst.end())
                lst.erase(elem_it);

            if (lst.empty()) {
                // could emplace_back and std::move improve performance for big
                // objects?
                S.push_back(key);
                // Remove already sorted nodes
                it_graph = graph_.erase(it_graph);
            } else {
                ++it_graph;
            }
        }

        S.pop_front();
        return sorted_elem;
    }

    /**
     * @brief Checks if there are still nodes in the queue
     *
     * @return true/false there are still nodes available
     */
    bool has_next() const {
        return S.size() > 0;
    }

    /**
     * @brief Sort all elements and return the sorted list
     *
     * @return topologically sorted list of nodes
     */
    std::vector<Node> & sort() {
        // as long as the list S is not empty
        // take a key out of it and put it into the sorted list L
        // find nodes without successors and put them into a list
        while (has_next()) {
            L.push_back(next());
        }

        if (graph_.size() > 0)
            throw std::invalid_argument("Input graph is not acyclic");

        return L;
    }

    /**
     * @brief Creates a reverse graph from an existing one
     *
     * @param graph
     *
     * @return the reversed graph
     */
    static MapType reverse(const MapType & graph) {
        MapType reverse_graph;

        for (const auto & p : graph) {
            for (const auto & elem: p.second)
                reverse_graph[elem].insert(p.first);
        }

        return reverse_graph;
    }

private:

    void initialize() {
        std::set<Node> all_nodes;
        std::set<Node> seen_nodes;

        // find nodes without successors and put them into a list S
        for (auto it = graph_.begin(); it != graph_.end();) {
            auto & key = it->first;
            auto & lst = it->second;

            // we don't know if all nodes have a key in the map
            seen_nodes.insert(key);
            all_nodes.insert(lst.begin(), lst.end());

            if (it->second.empty()) {
                S.push_back(key);
                // Remove already sorted nodes
                it = graph_.erase(it);
            } else {
                ++it;
            }
        }
        // iterate over all_nodes if one of it is not represented in the keys
        // it has no dependencies
        for (const auto &v: all_nodes) {
            if (seen_nodes.find(v) == seen_nodes.end())
                S.push_back(v);
        }
    }

    MapType           graph_;
    std::list<Node>   S;
    std::vector<Node> L;
};


template <typename MapType>
std::shared_ptr<Topsort<MapType>>
make_topsort(MapType && graph_) {
    return std::make_shared<Topsort<MapType>>(std::forward<MapType>(graph_));
}

template <typename MapType>
std::shared_ptr<Topsort<MapType>>
make_topsort(const MapType & graph_) {
    return std::make_shared<Topsort<MapType>>(graph_);
}

}
