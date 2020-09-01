#pragma once

#include "Tree.hpp"
#include "TreeAdapters.hpp"

#include<memory>

/**
 * @brief general interface for command
 */
class Command {
public:
    /**
     * @briefs executes command
     */
    virtual void call() = 0;

    /**
     * @briefs cancel command execution
     */
    virtual void undo() = 0;
};

/**
 * @brief enumeration of possible tree actions
 */
enum class TreeAction {
    insert, remove
};

/**
 * @brief a command that allows to interact with tree
 * @tparam T a type of data stored in tree
 */
template<typename T>
class TreeCommand : Command {
public:
    TreeCommand() = delete;

    TreeCommand(std::shared_ptr<Tree<T>> tree, const TreeAction action, const T &key);

    /**
     * @briefs executes command
     */
    void call() override;

    /**
     * @briefs cancel command execution
     * @note works as undo to a tree content;
     *       calling call() and undo() methods with equal arguments coherently can change tree structure;
     */
    void undo() override;

    /**
     * @return tree argument of command
     */
    auto tree() -> std::shared_ptr<Tree<T>>;

    /**
     * @return a key to be proceed by command
     */
    auto key() -> T;

    /**
     * @return tree action of command
     */
    auto action() -> TreeAction;

    /**
     * @return true if command was executed and was not undone
     */
    auto executed() -> bool;

private:
    std::shared_ptr<Tree<T>> _tree;
    T _key;
    TreeAction _action;
    bool _executed;
};

template<typename T>
TreeCommand(std::shared_ptr<Tree<T>>, TreeAction, const T &) -> TreeCommand<typename Tree<T>::value_type>;

#include "TreeCommands.hxx"
