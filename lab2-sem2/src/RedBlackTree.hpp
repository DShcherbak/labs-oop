#pragma once

#include <memory>

/**
 * a class for storing red-black tree data structure
 * @tparam T a type of data stored in tree
 */
template<typename T>
class RedBlackTree {
public:
    using value_type = T;

private:
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
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;
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

private:

    void _insertFixup(std::shared_ptr<Node> node);

    void _deleteFixup(std::shared_ptr<Node> node);

    /**
     * @brief replaces dst node with src
     * @param dst destination node (with all its branches)
     * @param src source node
     */
    void _transplant(std::shared_ptr<Node> dst, std::shared_ptr<Node> src);

    /**
     * @brief finds a node with min value in subtree
     * @param subtree_root root of subtree being processed
     * @return a node with min value in subtree with given root
     */
    auto _subtreeMinNode(std::shared_ptr<Node> subtree_root) -> std::shared_ptr<Node>;

    /**
     * @brief finds a node with max value in subtree
     * @param subtree_root root of subtree being processed
     * @return a node with max value in subtree with given root
     */
    auto _subtreeMaxNode(std::shared_ptr<Node> subtree_root) -> std::shared_ptr<Node>;

    /**
     * @brief finds a node with successor value
     * @param node a node of tree
     * @return a node with value, that is a successor of value in given node
     */
    auto _nodeSuccessor(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;

    /**
     * @brief finds a node with predecessor value
     * @param node a node of tree
     * @return a node with value? that is a predecessor of value in given node
     */
    auto _nodePredecessor(std::shared_ptr<Node> node) -> std::shared_ptr<Node>;

    auto _searchImpl(std::shared_ptr<Node> node, const value_type &key) -> std::shared_ptr<Node>;

    void _leftRotate(std::shared_ptr<Node> node);

    void _rightRotate(std::shared_ptr<Node> node);

    std::shared_ptr<Node> _root;
    std::shared_ptr<Node> _null_node;
};



#include "RedBlackTree.hxx"