#pragma once

#include "visitor/Visitor.h"
#include "../gui/tree-gui/drawingNode.h"
#include <memory>
#include <tuple>

/**
 * included for quick debug output
 * TODO: remove
 */
#include <iostream>

/**
 * @brief a class for storing red-black tree data structure
 * @tparam T a type of data stored in tree
 */
template<typename T>
class RedBlackTree {
public:
    using value_type = T;

protected:
    /**
     * @brief tree node structure
     */
    struct Node {
        enum class Color {
            RED, BLACK
        };

        explicit Node(const value_type &key);

        void switchColor();

        Color color;
        value_type key;
        Node *left;
        Node *right;
        Node *parent;
    };

public:
    RedBlackTree();

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
    };

    /**
     * @brief accepts a visitor, which generates a graphic interpretation of a tree
     * @param visitor a visitor to be accepted
     */
    template<class Widget>
    void acceptVisitor(Visitor<Widget>& visitor);

private:
    //temporary solution for quick testing
    //TODO:: remove;
    template<typename OStream>
    void _print(OStream &output, Node *node, int level = 0) {

        output << '|';
        for (std::size_t i = 0; i < level; i++) {
            output << "\t|";
        }

        if (node == _null_node) {
            output << "*\n";
            return;
        } else {
            output << node->key << " (";

            if (node->color == Node::Color::BLACK) {
                output << "B";
            } else {
                output << "R";
            }

            output << ")\n";
        }
        _print(output, node->left, level + 1);
        _print(output, node->right, level + 1);
    }

    /**
     * @brief restores rb-tree features after key insertion
     * @param node a node of tree
     */
    void _insertFixup(Node *node);

    /**
     * @brief restores rb-tree features after key removal
     * @param node a node of tree
     */
    void _deleteFixup(Node *node);

    /**
     * @brief replaces dst node with src
     * @param dst destination node (with all its branches)
     * @param src source node
     */
    void _transplant(Node *dst, Node *src);

    /**
     * @brief finds a node with min value in subtree
     * @param subtree_root root of subtree being processed
     * @return a node with min value in subtree with given root
     */
    auto _subtreeMinNode(Node *subtree_root) -> Node *;

    /**
     * @brief finds a node with max value in subtree
     * @param subtree_root root of subtree being processed
     * @return a node with max value in subtree with given root
     */
    auto _subtreeMaxNode(Node *subtree_root) -> Node *;

    /**
     * @brief finds a node with successor value
     * @param node a node of tree
     * @return a node with value, that is a successor of value in given node
     */
    auto _nodeSuccessor(Node *node) -> Node *;

    /**
     * @brief finds a node with predecessor value
     * @param node a node of tree
     * @return a node with value? that is a predecessor of value in given node
     */
    auto _nodePredecessor(Node *node) -> Node *;

    /**
     * @brief an implementation of search operation
     * @param node a candidate node
     * @param key a key to be searched
     * @return a node where search is stopped (equal to _null_node if key was not found)
     */
    auto _searchImpl(Node *node, const value_type &key) -> Node *;

    /**
     * @brief an implementation of node removal
     * @param node a node to remove
     */
    void _removeImpl(Node *node);

    /**
     * @brief left rotate method for tree balancing
     * @param node_x a node of tree
     */
    void _leftRotate(Node *node_x);

    /**
     * @brief right rotate method for tree balancing
     * @param node_x a node of tree
     */
    void _rightRotate(Node *node_x);

protected:
    Node *_root;
    Node *_null_node;
};

/**
 * @brief a decorated RedBlackTree class with methods for checking the correctness of constructed structure
 * @tparam T a type of data stored in tree
 */
template<typename T>
class RedBlackTreeTestable : public RedBlackTree<T> {
public:
    RedBlackTreeTestable();

    /**
     * @brief checks if keys are stored correctly in tree
     * @return true if the rule for storing values is satisfied
     *         (keys smaller than node's key are stored in left subtree, bigger - in right subtree),
     *         else - false
     */
    [[nodiscard]] bool checkValues();

    /**
     * @brief checks if nodes of tree have correct colors
     * @return true if rules for nodes' colors are satisfied
     *         (1. root and all leaves (nil) are black; 2. if node is red, both of its children are black),
     *         else - false
     */
    [[nodiscard]] bool checkColors();

    /**
     * @brief checks if all black heights of leaves are equal
     * @return true if the rule for black height is satisfied, else - false
     */
    [[nodiscard]] bool checkBlackHeights();

    /**
     * @brief runs all possible checks for tree
     * @return true if all checks passed successfully, else - false
     */
    [[nodiscard]] bool complexCheck();

private:
    using Node = typename RedBlackTree<T>::Node;

    /**
     * @brief an implementation of checking keys storing in tree
     * @param node a node of tree
     * @return a tuple that contains a bool result of rule satisfaction check for given node,
     *         a pointer to node with the smallest key in a subtree with a root in given node and
     *         a pointer to node with the greatest key in a subtree with a root in given node
     */
    [[nodiscard]] auto _checkValuesImpl(Node *node) -> std::tuple<bool, Node *, Node *>;

    /**
     * @brief an implementation of checking nodes' colors
     * @param node a node of tree
     * @return true if rule for colors is satisfied for given node, its left and right subtrees, else - false
     */
    [[nodiscard]] bool _checkColorsImpl(Node *node);

    /**
     * @brief an implementation of checking rule on black heights
     * @param node a node of tree
     * @return a pair of a bool result of rule satisfaction for given node and a black height of given node
     */
    [[nodiscard]] auto _checkBlackHeightsImpl(Node *node) -> std::pair<bool, std::size_t>;
};

#include "RedBlackTree.hxx"