#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include "BTree.hpp"


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
bool BTree<T>::includes(const value_type &key) {
    return _searchImpl(_root, key);
}

template<typename T>
void BTree<T>::insert(const value_type &key) {
    _insertImpl(key);
}

template<typename T>
void BTree<T>::remove(const value_type &key) {
    _removeImpl(_root, key);
}

template<typename T>
T BTree<T>::_subtreeMinValue(std::shared_ptr<Node> subtree_root) {
    if (subtree_root->is_leaf) {
        return subtree_root->keys.front();
    } else {
        return _subtreeMinValue(subtree_root->children.front());
    }
}

template<typename T>
T BTree<T>::_subtreeMaxValue(std::shared_ptr<Node> subtree_root) {
    if (subtree_root->is_leaf) {
        return subtree_root->keys.back();
    } else {
        return _subtreeMaxValue(subtree_root->children.back());
    }
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
        for (auto &item : new_child->children) {
            item->parent = new_child;
        }
        child->children.erase(child->children.cbegin() + median_index + 1, child->children.cend());
    }

    node->keys.insert(node->keys.cbegin() + child_index, child->keys[median_index]);
    node->children.insert(node->children.cbegin() + child_index + 1, new_child);

    child->keys.erase(child->keys.cbegin() + median_index, child->keys.cend());
}

template<typename T>
void BTree<T>::_mergeChildren(std::shared_ptr<Node> node,
                              std::size_t separator_index) {
    auto &left_child = node->children[separator_index];
    auto &right_child = node->children[separator_index + 1];

    left_child->keys.push_back(node->keys[separator_index]);
    left_child->keys.resize(left_child->keys.size() + right_child->keys.size());
    std::move(right_child->keys.cbegin(),
              right_child->keys.cend(),
              left_child->keys.end() - right_child->keys.size());

    for (auto &item : right_child->children) {
        item->parent = left_child;
    }
    left_child->children.resize(left_child->children.size() + right_child->children.size());
    std::move(right_child->children.cbegin(),
              right_child->children.cend(),
              left_child->children.end() - right_child->children.size());

    node->keys.erase(node->keys.begin() + separator_index);
    node->children.erase(node->children.begin() + separator_index + 1);

    if (node == _root && node->size() == 0) {
        left_child->parent.reset();
        _root = left_child;
    }
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
        if (destination_child->size() == _max_node_fill) {
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
    auto root = _root;
    if (root->size() == _max_node_fill) {
        auto new_root = std::make_shared<Node>();

        new_root->is_leaf = false;
        new_root->children.push_back(root);
        root->parent = new_root;

        _root = new_root;

        _splitChild(new_root, root, 0);
        _insertNonFull(new_root, key);
    } else {
        _insertNonFull(root, key);
    }
}

template<typename T>
void BTree<T>::_removeImpl(std::shared_ptr<Node> node,
                           const value_type &key) {
    auto key_it = std::find(node->keys.cbegin(), node->keys.cend(), key);
    if (node->is_leaf) {
        if (key_it != node->keys.cend()) {
            node->keys.erase(key_it);
        }
    } else {
        if (key_it != node->keys.cend()) {

            std::size_t key_index = std::distance(node->keys.cbegin(), key_it);
            auto &prev_child = node->children[key_index];
            auto &next_child = node->children[key_index + 1];

            if (prev_child->size() > _min_node_fill) {
                auto key_predecessor = _subtreeMaxValue(prev_child);
                _removeImpl(prev_child, key_predecessor);
                node->keys[key_index] = key_predecessor;

            } else if (next_child->size() > _min_node_fill) {
                auto key_successor = _subtreeMinValue(next_child);
                _removeImpl(next_child, key_successor);
                node->keys[key_index] = key_successor;

            } else {
                _mergeChildren(node, key_index);
                _removeImpl(prev_child, key);
            }

        } else {
            auto dest_child_index =
                    std::distance(node->keys.cbegin(),
                                  std::find_if(node->keys.cbegin(), node->keys.cend(),
                                               [key = key](const auto &item) { return item >= key; }
                                  )
                    );
            auto dest_child = node->children[dest_child_index];

            if (dest_child->size() == _min_node_fill) {

                bool dest_is_leftmost = (dest_child_index == 0);
                bool dest_is_rightmost = (dest_child_index == node->children.size() - 1);

                if (!dest_is_leftmost && node->children[dest_child_index - 1]->size() > _min_node_fill) {
                    auto &left_sibling = node->children[dest_child_index - 1];
                    auto separator_index = dest_child_index - 1;

                    dest_child->keys.insert(dest_child->keys.begin(), node->keys[separator_index]);
                    node->keys[separator_index] = left_sibling->keys.back();
                    left_sibling->keys.pop_back();

                    if (!dest_child->is_leaf) {
                        left_sibling->children.back()->parent = dest_child;
                        dest_child->children.insert(dest_child->children.begin(),
                                                    left_sibling->children.back());
                        left_sibling->children.pop_back();
                    }

                } else if (!dest_is_rightmost && node->children[dest_child_index + 1]->size() > _min_node_fill) {
                    auto &right_sibling = node->children[dest_child_index + 1];
                    auto separator_index = dest_child_index;

                    dest_child->keys.push_back(node->keys[separator_index]);
                    node->keys[separator_index] = right_sibling->keys.front();
                    right_sibling->keys.erase(right_sibling->keys.begin());

                    if (!dest_child->is_leaf) {
                        right_sibling->children.front()->parent = dest_child;
                        dest_child->children.push_back(right_sibling->children.front());
                        right_sibling->children.erase(right_sibling->children.begin());
                    }

                } else {
                    if (!dest_is_rightmost) {
                        _mergeChildren(node, dest_child_index);
                    } else {
                        _mergeChildren(node, dest_child_index - 1);
                        dest_child = node->children[dest_child_index - 1];
                    }
                }
            }

            _removeImpl(dest_child, key);
        }
    }
}

template<typename T>
bool BTree<T>::_searchImpl(std::shared_ptr<Node> node, const value_type &key) {
    auto key_it = std::find(node->keys.cbegin(), node->keys.cend(), key);
    if (node->is_leaf) {
        return key_it != node->keys.cend();
    } else {
        if (key_it != node->keys.cend()) {
            return true;
        }

        auto dest_child_index =
                std::distance(node->keys.cbegin(),
                              std::find_if(node->keys.cbegin(), node->keys.cend(),
                                           [key = key](const auto &item) { return item > key; }
                              )
                );
        return _searchImpl(node->children[dest_child_index], key);
    }
}

template<typename T>
template<class Visitor>
void BTree<T>::acceptVisitor(Visitor visitor) {
    visitor.visitBTree(_root);
}

template<typename T>
BTreeTestable<T>::BTreeTestable(std::size_t min_degree) : BTree<T>(min_degree) {}

template<typename T>
bool BTreeTestable<T>::checkValues() {
    return _checkValuesImpl(this->_root, std::nullopt, std::nullopt);
}

template<typename T>
bool BTreeTestable<T>::checkFilling() {
    return _checkFillingImpl(this->_root);
}

template<typename T>
bool BTreeTestable<T>::checkHeights() {
    return _checkHeightsImpl(this->_root).first;
}

template<typename T>
bool BTreeTestable<T>::complexCheck() {
    return checkValues() && checkFilling() && checkHeights();
}

template<typename T>
bool BTreeTestable<T>::_checkValuesImpl(std::shared_ptr<Node> node,
                                        const std::optional<T> &lower_bound,
                                        const std::optional<T> &upper_bound) {
    bool res = true;
    for (std::size_t i = 1; i < node->size(); i++) {
        res &= node->keys[i - 1] <= node->keys[i];
    }

    if (lower_bound.has_value()) {
        res &= (node->keys.front() >= lower_bound.value());
    }

    if (upper_bound.has_value()) {
        res &= (node->keys.back() <= upper_bound.value());
    }

    if (!res) {
        return false;
    }

    if (!node->is_leaf) {
        for (std::size_t i = 1; i < node->size(); i++) {
            res &= _checkValuesImpl(node->children[i], node->keys[i - 1], node->keys[i]);
        }
        res &= _checkValuesImpl(node->children.front(), lower_bound, node->keys.front());
        res &= _checkValuesImpl(node->children.back(), node->keys.back(), upper_bound);
    }

    return res;
}

template<typename T>
bool BTreeTestable<T>::_checkFillingImpl(std::shared_ptr<Node> node) {
    bool res;

    if (node == this->_root) {
        res = (node->size() <= this->_max_node_fill);
    } else {
        res = (node->size() >= this->_min_node_fill) && (node->size() <= this->_max_node_fill);
    }

    if (!node->is_leaf) {
        for (const auto &child : node->children) {
            if (!res) {
                return false;
            }
            res &= _checkFillingImpl(child);
        }
    }

    return res;
}

template<typename T>
auto BTreeTestable<T>::_checkHeightsImpl(std::shared_ptr<Node> node) -> std::pair<bool, std::size_t> {
    if (node->is_leaf) {
        return {true, 1};
    }

    auto [res, height] = _checkHeightsImpl(node->children[0]);
    for (auto it = node->children.begin() + 1; it != node->children.end(); it++) {
        if (!res) {
            return {false, 0};
        }

        auto [res_step, height_step] = _checkHeightsImpl(*it);
        res &= res_step && (height == height_step);
    }

    return {res, height};
}
