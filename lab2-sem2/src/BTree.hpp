#pragma once

#include <memory>
#include <vector>
#include <tuple>
#include <optional>

/**
 * included for quick debug output
 * TODO: remove
 */
#include <iostream>

constexpr std::size_t MIN_DEGREE = 2;

/**
 * @brief a class for storing b-tree data structure
 * @tparam T a type of data stored in tree
 */
template<typename T>
class BTree {
public:
    using value_type = T;

protected:
    /**
     * @brief tree node structure
     */
    struct Node {
        Node();

        /**
         * @return number of keys stored in node
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * @return number of node's children
         */
        [[nodiscard]] std::size_t childrenNumber() const;

        std::vector<value_type> keys;
        std::vector<std::shared_ptr<Node>> children;
        bool is_leaf;
        std::weak_ptr<Node> parent;
    };

public:
    explicit BTree(std::size_t min_degree = MIN_DEGREE);

    /**
     * @return min degree parameter of tree
     */
    [[nodiscard]] std::size_t minDegree();

    /**
     * @brief checks if key was added to tree
     * @param key a key to check
     * @return true if key is in tree, else - false
     */
    [[nodiscard]] bool includes(const value_type &key);

    /**
     * @brief inserts key to tree
     * @param key a key to be inserted
     */
    void insert(const value_type &key);

    /**
     * @brief removes key from tree
     * @param key a key to be removed
     */
    void remove(const value_type &key);

    //temporary solution for quick testing
    //TODO:: remove;
    void print() {
        _print(std::cout, _root);
    }

    /**
     * @brief accepts a visitor, which generates a graphic interpretation of a tree
     * @param visitor a visitor to be accepted
     */
    template<class Visitor>
    void acceptVisitor(Visitor visitor);

private:
    //temporary solution for quick testing
    //TODO:: remove;
    template<typename OStream>
    void _print(OStream &output, std::shared_ptr<Node> node, int level = 0) {
        output << '|';
        for (std::size_t i = 0; i < level; i++) {
            output << '\t' << '|';
        }

        if (!node) {
            output << "*\n";
        } else {
            output << "Data: (" << node->size() << ")\n";

            for (auto &item : node->keys) {
                output << '|';
                for (std::size_t i = 0; i < level; i++) {
                    output << '\t' << '|';
                }
                output << item << "\n";
            }

            if (!node->is_leaf) {
                output << '|';
                for (std::size_t i = 0; i < level; i++) {
                    output << '\t' << '|';
                }
                output << "Children:\n";
                for (auto &item : node->children) {
                    _print(output, item, level + 1);
                }
            }
        }

        output << '|';
        for (std::size_t i = 0; i < level; i++) {
            output << '\t' << '|';
        }
        output << "=============\n";
    }

    /**
     * @param subtree_root root of subtree being processed
     * @return min value stored in subtree
     */
    value_type _subtreeMinValue(std::shared_ptr<Node> subtree_root);

    /**
     * @param subtree_root root of subtree being processed
     * @return max value stored in subtree
     */
    value_type _subtreeMaxValue(std::shared_ptr<Node> subtree_root);

    /**
     * @brief splits node's child to separate nodes to avoid overflow
     * @param node the node with full child
     * @param child the full node
     * @param child_index index of full child
     */
    void _splitChild(std::shared_ptr<Node> node,
                     std::shared_ptr<Node> child,
                     std::size_t child_index);

    /**
     * @brief merges two node's children separated by single key
     * @param node a parent node of mergeable children
     * @param separator_index index of key - separator of mergeable children
     */
    void _mergeChildren(std::shared_ptr<Node> node,
                        std::size_t separator_index);

    /**
     * @brief inserts new key to the tree with non full root
     * @param node a candidate node for key insertion
     * @param key new key to be inserted
     */
    void _insertNonFull(std::shared_ptr<Node> node,
                        const value_type &key);

    /**
     * @brief implementation of new key insertion
     * @param key new key to be inserted
     */
    void _insertImpl(const value_type &key);

    /**
     * @brief implementation of key removal
     * @param node a candidate node for key removal
     * @param key key to be removed
     */
    void _removeImpl(std::shared_ptr<Node> node,
                     const value_type &key);

    /**
     * @brief implementation of key search
     * @param node a candidate node for key search
     * @param key searched key
     * @return true if key is in node, else - false
     */
    bool _searchImpl(std::shared_ptr<Node> node,
                     const value_type &key);

protected:
    std::size_t _min_degree;
    std::size_t _min_node_fill;
    std::size_t _max_node_fill;
    std::shared_ptr<Node> _root;
};

/**
 * @brief a decorated BTree class with methods for checking the correctness of constructed structure
 * @tparam T a type of data stored in tree
 */
template<typename T>
class BTreeTestable : public BTree<T> {
public:
    explicit BTreeTestable(std::size_t min_degree = MIN_DEGREE);

    /**
     * @brief checks if keys are stored correctly in tree
     * @return true if keys are stored correctly, else - false
     */
    [[nodiscard]] bool checkValues();

    /**
     * @brief checks number of keys stored in nodes
     * @return true if all nodes are filled enough and there is no overflow, else - false
     */
    [[nodiscard]] bool checkFilling();

    /**
     * @brief checks heights in tree
     * @return true if all leaves have equal heights, else - false
     */
    [[nodiscard]] bool checkHeights();

    /**
     * @brief runs all possible checks for tree
     * @return true if all checks passed successfully, else - false
     */
    [[nodiscard]] bool complexCheck();

private:
    using Node = typename BTree<T>::Node;

    /**
     * @brief an implementation of checking keys storing in tree
     * @param node a node of tree
     * @param lower_bound a value expected to be lower than any key stored in the given node
     * @param upper_bound a value expected to be greater than any key stored in the given node
     * @return true if all rules on keys storing are satisfied, else - false
     */
    [[nodiscard]] bool _checkValuesImpl(std::shared_ptr<Node> node,
                                        const std::optional<T> &lower_bound,
                                        const std::optional<T> &upper_bound);

    /**
     * @brief an implementation of node's filling check
     * @return true if node's filling is greater than required value and smaller than maximum allowed, else - false
     */
    [[nodiscard]] bool _checkFillingImpl(std::shared_ptr<Node> node);

    /**
     * @brief an implementation of heights check
     * @param node a node of tree
     * @return a pair of bool result of checking rules on height in given node and height of given node
     */
    [[nodiscard]] auto _checkHeightsImpl(std::shared_ptr<Node> node) -> std::pair<bool, std::size_t>;
};

#include "BTree.hxx"
