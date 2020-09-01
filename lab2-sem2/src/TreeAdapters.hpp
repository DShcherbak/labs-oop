#pragma once

#include "Tree.hpp"
#include "RedBlackTree.hpp"
#include "BTree.hpp"

#include <memory>

/**
 * @brief an adapter for red-black tree
 * @tparam T a type of data stored in tree
 */
template<typename T>
class RedBlackTreeAdapter : public Tree<T> {
public:
    RedBlackTreeAdapter();

    /**
     * @brief inserts a new key tree
     * @param key a key to insert
     */
    void insert(const T &key) override;

    /**
     * @brief removes a key from tree
     * @param key a key to remove
     */
    void remove(const T &key) override;

    /**
     * @brief checks if tree contains key
     * @param key a key to check
     * @return true if key was added to tree earlier, else - false
     */
    bool contains(const T &key) override;

    //temporary solution
    //TODO: remove;
    void print() override {
        _tree->print();
    }

    ~RedBlackTreeAdapter() = default;

private:
    std::unique_ptr<RedBlackTree<T>> _tree;
};

/**
 * @brief an adapter for b-tree
 * @tparam T a type of data stored in tree
 */
template<typename T>
class BTreeAdapter : public Tree<T> {
public:
    BTreeAdapter();

    explicit BTreeAdapter(std::size_t min_degree);

    /**
     * @brief inserts a new key tree
     * @param key a key to insert
     */
    void insert(const T &key) override;

    /**
     * @brief removes a key from tree
     * @param key a key to remove
     */
    void remove(const T &key) override;

    /**
     * @brief checks if tree contains key
     * @param key a key to check
     * @return true if key was added to tree earlier, else - false
     */
    bool contains(const T &key) override;

    //temporary solution
    //TODO: remove;
    void print() override {
        _tree->print();
    }

    ~BTreeAdapter() = default;

private:
    std::unique_ptr<BTree<T>> _tree;
};

#include "TreeAdapters.hxx"