#pragma once

#include "TreeCommands.hpp"

#include "chrono"

template<typename T>
TreeCommand<T>::TreeCommand(std::shared_ptr<Tree<T>> tree, const TreeAction action, const T &key) :
        _tree{tree},
        _action{action},
        _key{key},
        _executed{false} {}


template<typename T>
void TreeCommand<T>::call() {
    switch (_action) {
        case TreeAction::insert:
            _tree->insert(_key);
            _executed = true;
            break;
        case TreeAction::remove:
            _executed = _tree->contains(_key);
            _tree->remove(_key);
            break;
    }
}

template<typename T>
void TreeCommand<T>::undo() {
    if (!_executed) return;

    switch (_action) {
        case TreeAction::insert:
            _tree->remove(_key);
            break;
        case TreeAction::remove:
            _tree->insert(_key);
    }

    _executed = false;
}

template<typename T>
auto TreeCommand<T>::tree() -> std::shared_ptr<Tree<T>> {
    return _tree;
}

template<typename T>
auto TreeCommand<T>::key() -> T {
    return _key;
}

template<typename T>
auto TreeCommand<T>::action() -> TreeAction {
    return _action;
}

template<typename T>
auto TreeCommand<T>::executed() -> bool {
    return _executed;
}
