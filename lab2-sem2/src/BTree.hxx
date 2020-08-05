#pragma once

#include <memory>
#include <vector>
#include <algorithm>

template<typename T>
BTree<T>::Node::Node() :
        is_leaf{false},
        parent{} {}

template<typename T>
std::size_t BTree<T>::Node::size() const {
    return keys.size();
}

template<typename T>
std::size_t BTree<T>::Node::childrenNumber() const {
    return children.size();
}

template<typename T>
BTree<T>::BTree(std::size_t min_degree) :
        _min_degree{min_degree < MIN_DEGREE ? MIN_DEGREE : min_degree},
        _min_node_fill{_min_degree - 1},
        _max_node_fill{_min_degree * 2 - 1},
        _root{std::make_shared<Node>()} {
    _root->is_leaf = true;
}

template<typename T>
std::size_t BTree<T>::minDegree() {
    return _min_degree;
}

template<typename T>
void BTree<T>::_splitChild(std::shared_ptr<Node> node,
                           std::shared_ptr<Node> child,
                           std::size_t child_index) {
    auto new_child = std::make_shared<Node>();
    new_child->is_leaf = child->is_leaf;
    new_child->parent = node;

    std::size_t median_index = child->size() / 2;

    new_child->keys.resize(child->size() - median_index - 1);
    std::move(child->keys.cbegin() + median_index + 1, child->keys.cend(), new_child->keys.begin());

    if (!new_child->is_leaf) {
        new_child->children.resize(new_child->size() + 1);
        std::move(child->children.cbegin() + median_index + 1, child->children.cend(), new_child->children.begin());
        child->children.erase(child->children.cbegin() + median_index + 1, child->children.cend());
    }

    node->keys.insert(node->keys.cbegin() + child_index, child->keys[median_index]);
    node->children.insert(node->children.cbegin() + child_index + 1, new_child);

    child->keys.erase(child->keys.cbegin() + median_index, child->keys.cend());
}

template<typename T>
void BTree<T>::_insertNonFull(std::shared_ptr<Node> node,
                              const value_type &key) {
    if (node->is_leaf) {
        auto insertion_pos_it =
                std::find_if(node->keys.cbegin(), node->keys.cend(),
                             [key = key](const auto &item) { return item >= key; }
                );
        node->keys.insert(insertion_pos_it, key);
    } else {
        auto destination_child_index =
                std::distance(node->keys.cbegin(),
                              std::find_if(node->keys.cbegin(), node->keys.cend(),
                                           [key = key](const auto &item) { return item >= key; }
                              )
                );

        auto destination_child = node->children[destination_child_index];
        if (destination_child->size() == this->_max_node_fill) {
            _splitChild(node, destination_child, destination_child_index);
            if (key > node->keys[destination_child_index]) {
                destination_child = node->children[destination_child_index + 1];
            }
        }

        _insertNonFull(destination_child, key);
    }
}

template<typename T>
void BTree<T>::_insertImpl(const value_type &key) {
    auto root = this->_root;
    if (root->size() == this->_max_node_fill) {
        auto new_root = std::make_shared<Node>();

        new_root->is_leaf = false;
        new_root->children.push_back(root);
        root->parent = new_root;

        this->_root = new_root;

        _splitChild(new_root, root, 0);
        _insertNonFull(new_root, key);
    } else {
        _insertNonFull(root, key);
    }
}
