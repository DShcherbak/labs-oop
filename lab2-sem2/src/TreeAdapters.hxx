#pragma once

#include "TreeAdapters.hpp"

template<typename T>
RedBlackTreeAdapter<T>::RedBlackTreeAdapter() :
        Tree<T>{},
        _tree{std::make_unique<RedBlackTree<T>>()} {};

template<typename T>
void RedBlackTreeAdapter<T>::insert(const T &key) {
    _tree->insert(std::move(key));
};

template<typename T>
void RedBlackTreeAdapter<T>::remove(const T &key) {
    _tree->remove(std::move(key));
};

template<typename T>
bool RedBlackTreeAdapter<T>::contains(const T &key) {
    return _tree->includes(std::move(key));
};

template<typename T>
BTreeAdapter<T>::BTreeAdapter() : BTreeAdapter{2} {}


template<typename T>
BTreeAdapter<T>::BTreeAdapter(std::size_t min_degree) :
        Tree<T>{},
        _tree{std::make_unique<BTree<T>>(min_degree)} {};

template<typename T>
void BTreeAdapter<T>::insert(const T &key) {
    _tree->insert(std::move(key));
};

template<typename T>
void BTreeAdapter<T>::remove(const T &key) {
    _tree->remove(std::move(key));
};

template<typename T>
bool BTreeAdapter<T>::contains(const T &key) {
    return _tree->includes(std::move(key));
};