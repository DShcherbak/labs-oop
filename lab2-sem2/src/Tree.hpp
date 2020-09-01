#pragma once

/**
 * @brief a general interface for tree data structure
 * @tparam T a type of data stored in tree
 */
template<typename T>
class Tree {
public:
    using value_type = T;

    /**
     * @brief inserts a new key tree
     * @param key a key to insert
     */
    virtual void insert(const T &key) = 0;

    /**
     * @brief removes a key from tree
     * @param key a key to remove
     */
    virtual void remove(const T &key) = 0;

    /**
     * @brief checks if tree contains key
     * @param key a key to check
     * @return true if key was added to tree earlier, else - false
     */
    virtual bool contains(const T &key) = 0;

    //temporary solution
    //TODO: remove;
    virtual void print() = 0;

    virtual ~Tree() = default;
};

