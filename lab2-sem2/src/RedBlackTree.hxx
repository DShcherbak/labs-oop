#pragma once

#include "RedBlackTree.hpp"

template<typename T>
RedBlackTree<T>::Node::Node(const value_type &key) :
        key{key},
        color{Color::RED},
        left{nullptr},
        right{nullptr},
        parent{nullptr} {}

template<typename T>
void RedBlackTree<T>::Node::switchColor() {
    if (color == Color::BLACK) {
        color = Color::RED;
    } else {
        color = Color::BLACK;
    }
}

template<typename T>
RedBlackTree<T>::RedBlackTree() :
        _null_node{new Node(value_type())} {
    _null_node->parent = _null_node->left = _null_node->right = _null_node;
    _null_node->color = Node::Color::BLACK;
    _root = _null_node;
}

template<typename T>
void RedBlackTree<T>::insert(const value_type &key) {
    auto node_y = _null_node;
    auto node_x = _root;

    while (node_x != _null_node) {
        node_y = node_x;

        if (key < node_x->key) {
            node_x = node_x->left;
        } else {
            node_x = node_x->right;
        }
    }

    auto new_node = new Node(key);
    new_node->parent = node_y;
    new_node->left = new_node->right = _null_node;
    if (node_y == _null_node) {
        _root = new_node;
    } else if (key < node_y->key) {
        node_y->left = new_node;
    } else {
        node_y->right = new_node;
    }

    _insertFixup(new_node);
}

template<typename T>
void RedBlackTree<T>::remove(const value_type &key) {
    auto node = _searchImpl(_root, key);
    if (node != _null_node) {
        _removeImpl(node);
    }
}

template<typename T>
bool RedBlackTree<T>::includes(const value_type &key) {
    return _searchImpl(_root, key) != _null_node;
}

template<typename T>
void RedBlackTree<T>::_removeImpl(Node *node) {
    auto *node_y = node;
    decltype(node) node_x;
    auto y_original_color = node_y->color;

    if (node->left == _null_node) {
        node_x = node->right;
        _transplant(node, node->right);
    } else if (node->right == _null_node) {
        node_x = node->left;
        _transplant(node, node->left);
    } else {
        node_y = _subtreeMinNode(node->right);
        y_original_color = node_y->color;
        node_x = node_y->right;
        if (node_y->parent == node) {
            node_x->parent = node_y;
        } else {
            _transplant(node_y, node_y->right);
            node_y->right = node->right;
            node_y->right->parent = node_y;
        }
        _transplant(node, node_y);
        node_y->left = node->left;
        node_y->left->parent = node_y;
        node_y->color = node->color;
    }
    delete node;

    if (y_original_color == Node::Color::BLACK) {
        _deleteFixup(node_x);
    }
}

template<typename T>
void RedBlackTree<T>::_insertFixup(Node *node) {
    while (node->parent->color == Node::Color::RED) {
        if (node->parent == node->parent->parent->left) {
            auto node_y = node->parent->parent->right;
            if (node_y->color == Node::Color::RED) {
                node->parent->color = Node::Color::BLACK;
                node_y->color = Node::Color::BLACK;
                node->parent->parent->color = Node::Color::RED;
                node = node->parent->parent;

            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    _leftRotate(node);
                }
                node->parent->color = Node::Color::BLACK;
                node->parent->parent->color = Node::Color::RED;
                _rightRotate(node->parent->parent);

            }

        } else {
            auto node_y = node->parent->parent->left;
            if (node_y->color == Node::Color::RED) {
                node->parent->color = Node::Color::BLACK;
                node_y->color = Node::Color::BLACK;
                node->parent->parent->color = Node::Color::RED;
                node = node->parent->parent;

            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    _rightRotate(node);
                }
                node->parent->color = Node::Color::BLACK;
                node->parent->parent->color = Node::Color::RED;
                _leftRotate(node->parent->parent);

            }
        }
    }

    _root->color = Node::Color::BLACK;
}

template<typename T>
void RedBlackTree<T>::_deleteFixup(Node *node) {
    while (node != _root && node->color == Node::Color::BLACK) {
        if (node == node->parent->left) {
            auto node_w = node->parent->right;
            if (node_w->color == Node::Color::RED) {
                node_w->color = Node::Color::BLACK;
                node->parent->color = Node::Color::RED;
                _leftRotate(node->parent);
                node_w = node->parent->right;
            }
            if (node_w->left->color == Node::Color::BLACK && node_w->right->color == Node::Color::BLACK) {
                node_w->color = Node::Color::RED;
                node = node->parent;
            } else {
                if (node_w->right->color == Node::Color::BLACK) {
                    node_w->left->color = Node::Color::BLACK;
                    node_w->color = Node::Color::RED;
                    _rightRotate(node_w);
                    node_w = node->parent->right;
                }
                node_w->color = node->parent->color;
                node->parent->color = Node::Color::BLACK;
                node_w->right->color = Node::Color::BLACK;
                _leftRotate(node->parent);
                node = _root;
            }
        } else {
            auto node_w = node->parent->left;
            if (node_w->color == Node::Color::RED) {
                node_w->color = Node::Color::BLACK;
                node->parent->color = Node::Color::RED;
                _rightRotate(node->parent);
                node_w = node->parent->left;
            }
            if (node_w->right->color == Node::Color::BLACK && node_w->left->color == Node::Color::BLACK) {
                node_w->color = Node::Color::RED;
                node = node->parent;
            } else {
                if (node_w->left->color == Node::Color::BLACK) {
                    node_w->right->color = Node::Color::BLACK;
                    node_w->color = Node::Color::RED;
                    _leftRotate(node_w);
                    node_w = node->parent->left;
                }
                node_w->color = node->parent->color;
                node->parent->color = Node::Color::BLACK;
                node_w->left->color = Node::Color::BLACK;
                _rightRotate(node->parent);
                node = _root;
            }
        }
    }
    node->color = Node::Color::BLACK;
}

template<typename T>
void RedBlackTree<T>::_transplant(Node *dst, Node *src) {
    if (dst->parent == _null_node) {
        _root = src;
    } else if (dst == dst->parent->left) {
        dst->parent->left = src;
    } else {
        dst->parent->right = src;
    }

    src->parent = dst->parent;
}

template<typename T>
auto RedBlackTree<T>::_subtreeMinNode(Node *subtree_root) -> Node * {
    auto ptr = subtree_root;

    while (ptr->left != _null_node) {
        ptr = ptr->left;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_subtreeMaxNode(Node *subtree_root) -> Node * {
    auto ptr = subtree_root;

    while (ptr->right != _null_node) {
        ptr = ptr->right;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_nodeSuccessor(Node *node) -> Node * {
    if (node->right != _null_node) {
        return _subtreeMinNode(node->right);
    }

    auto ptr = node->parent;
    while ((ptr != _null_node) && (node == ptr->right)) {
        node = ptr;
        ptr = ptr->parent;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_nodePredecessor(Node *node) -> Node * {
    if (node->left != _null_node) {
        return _subtreeMaxNode(node->left);
    }

    auto ptr = node->parent;
    while ((ptr != _null_node) && (node == ptr->left)) {
        node = ptr;
        ptr = ptr->parent;
    }

    return ptr;
}

template<typename T>
auto RedBlackTree<T>::_searchImpl(Node *node, const value_type &key) -> Node * {
    if ((node == _null_node) || (key == node->key)) {
        return node;
    }

    if (key < node->key) {
        return _searchImpl(node->left, key);
    } else {
        return _searchImpl(node->right, key);
    }
}

template<typename T>
void RedBlackTree<T>::_leftRotate(Node *node_x) {
    auto node_y = node_x->right;
    node_x->right = node_y->left;

    if (node_y->left != _null_node) {
        node_y->left->parent = node_x;
    }

    node_y->parent = node_x->parent;
    if (node_x->parent == _null_node) {
        _root = node_y;
    } else if (node_x == node_x->parent->left) {
        node_x->parent->left = node_y;
    } else {
        node_x->parent->right = node_y;
    }

    node_y->left = node_x;
    node_x->parent = node_y;
}

template<typename T>
void RedBlackTree<T>::_rightRotate(Node *node_x) {
    auto node_y = node_x->left;
    node_x->left = node_y->right;

    if (node_y->right != _null_node) {
        node_y->right->parent = node_x;
    }

    node_y->parent = node_x->parent;
    if (node_x->parent == _null_node) {
        _root = node_y;
    } else if (node_x == node_x->parent->left) {
        node_x->parent->left = node_y;
    } else {
        node_x->parent->right = node_y;
    }

    node_y->right = node_x;
    node_x->parent = node_y;
}

template<typename T>
template<class Widget>
void RedBlackTree<T>::acceptVisitor(const Visitor<Widget>& visitor) {
    visitor.visitRedBlackTree(_root);
}

template<typename T>
RedBlackTreeTestable<T>::RedBlackTreeTestable() : RedBlackTree<T>() {}

template<typename T>
bool RedBlackTreeTestable<T>::checkValues() {
    return std::get<0>(_checkValuesImpl(this->_root));
}

template<typename T>
bool RedBlackTreeTestable<T>::checkColors() {
    return this->_root->color == RedBlackTree<T>::Node::Color::BLACK &&
           this->_null_node->color == RedBlackTree<T>::Node::Color::BLACK &&
           _checkColorsImpl(this->_root);
}

template<typename T>
bool RedBlackTreeTestable<T>::checkBlackHeights() {
    return _checkBlackHeightsImpl(this->_root).first;
}

template<typename T>
bool RedBlackTreeTestable<T>::complexCheck() {
    return checkValues() && checkColors() && checkBlackHeights();
}

template<typename T>
auto RedBlackTreeTestable<T>::_checkValuesImpl(Node *node) -> std::tuple<bool, Node *, Node *> {
    if (node->left != this->_null_node && node->right != this->_null_node) {
        auto[left_check, left_min_node, left_max_node] = _checkValuesImpl(node->left);
        auto[right_check, right_min_node, right_max_node] = _checkValuesImpl(node->right);

        return std::make_tuple(
                left_check && right_check && (node->key >= left_max_node->key) && (node->key <= right_min_node->key),
                left_min_node,
                right_max_node
        );
    }

    if (node->right != this->_null_node) {
        auto[right_check, right_min_node, right_max_node] = _checkValuesImpl(node->right);
        return std::make_tuple(
                right_check && (node->key <= right_min_node->key),
                node,
                right_max_node
        );
    }

    if (node->left != this->_null_node) {
        auto[left_check, left_min_node, left_max_node] = _checkValuesImpl(node->left);
        return std::make_tuple(
                left_check && (node->key >= left_max_node->key),
                left_min_node,
                node
        );
    }

    return std::make_tuple(true, node, node);
}

template<typename T>
bool RedBlackTreeTestable<T>::_checkColorsImpl(Node *node) {
    if (node == this->_null_node) {
        return true;
    } else if (node->color == RedBlackTree<T>::Node::Color::BLACK) {
        return _checkColorsImpl(node->left) && _checkColorsImpl(node->right);
    } else {
        return node->left->color == RedBlackTree<T>::Node::Color::BLACK &&
               node->right->color == RedBlackTree<T>::Node::Color::BLACK &&
               _checkColorsImpl(node->left) &&
               _checkColorsImpl(node->right);
    }
}

template<typename T>
auto RedBlackTreeTestable<T>::_checkBlackHeightsImpl(Node* node) -> std::pair<bool, std::size_t> {
    if (node == this->_null_node) {
        return {true, 1};
    }

    auto [left_check, left_black_height] = _checkBlackHeightsImpl(node->left);
    auto [right_check, right_black_height] = _checkBlackHeightsImpl(node->right);

    if(left_check && right_check && (left_black_height == right_black_height)) {
        return {true, (node->color == Node::Color::BLACK ? left_black_height + 1 : left_black_height)};
    } else {
        return {false, 0};
    }
}