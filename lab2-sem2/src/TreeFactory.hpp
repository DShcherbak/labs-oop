#pragma once

#include "Tree.hpp"
#include "TreeAdapters.hpp"

#include <memory>

/**
 * tree factory for creating new tree objects
 */
class TreeFactory {
public:
    /**
     * @brief creates new b-tree
     * @tparam T a type of data stored in tree
     * @param min_degree a min degree parameter of b-tree
     * @return unique pointer to created tree object
     */
    template<typename T>
    inline static auto NewB (std::size_t min_degree = 2) -> std::unique_ptr<BTreeAdapter<T>> {
        return std::make_unique<BTreeAdapter<T>>(min_degree);
    }

    /**
     * @brief creates new red-black tree
     * @tparam T a type of data stored in tree
     * @return unique pointer to created tree object
     */
    template<typename T>
    inline static auto NewRedBlack () -> std::unique_ptr<RedBlackTreeAdapter<T>> {
        return std::make_unique<RedBlackTreeAdapter<T>>();
    }
};
