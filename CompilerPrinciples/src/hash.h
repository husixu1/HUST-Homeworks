/**
 * @file hash.h
 * @brief simple hash table implementation
 *
 * @details
 * This is a wrapper for stl's std::map, key is always string, and values are
 * template variables
 *
 * @date 2018-06-13
 * @author Sixu Hu
**/

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <map>
#include <string.h>

/// @brief string comparator class
struct ltstr {
    bool operator()(const char *s1, const char *s2) const {
        return strcmp(s1, s2) < 0;
    }
};

template <class ValueType> class Iterator;

/// @brief hash table definition
/// @tparam ValueType value type
template <class ValueType> class Hashtable {
private:
    /// @brief the internal map
    std::multimap<const char *, ValueType, ltstr> mmap;

public:
    /// @brief constructor
    Hashtable() {}

    // Returns number of entries currently in table
    /// @brief number of entrie in current table
    /// @return
    int size() const;

    /// @brief insert into hash table
    /// @param key key to insert
    /// @param value asscosiate value to insert
    /// @param overwrite overwrite the original value?
    void insert(const char *key, ValueType value,
            bool overwrite = true);

    /// @brief remove the key-val pair
    /// @param key key to remove
    /// @param value value to remove
    void remove(const char *key, ValueType value);

    /// @brief find value according to key
    /// @param key key to find
    /// @return value
    ValueType find(const char *key);

    /// @brief return an iterator
    Iterator<ValueType> GetIterator();

};

/// @brief iterator class for the hash table
/// @tparam ValueType value type
template<class ValueType> class Iterator {
private:
    /// @brief hashtable access to iterator internal implements
    friend class Hashtable<ValueType>;

    /// @brief iterator internals
    typename std::multimap<const char *, ValueType, ltstr>::iterator cur, end;

    /// @brief constroctor
    /// @param t the hash table
    Iterator(std::multimap<const char *, ValueType, ltstr> &t)
        : cur(t.begin()), end(t.end()) {}

public:
    /// @brief return current value and move iterator to next
    /// @return current iterator value
    ValueType next();
};

template <class ValueType> void Hashtable<ValueType>::insert(const char *key, ValueType val, bool overwrite) {
    ValueType prev;
    if (overwrite && (prev = find(key)))
        remove(key, prev);
    mmap.insert(std::make_pair(strdup(key), val));
}

template <class ValueType> void Hashtable<ValueType>::remove(const char *key, ValueType val) {
    if (mmap.count(key) == 0) // no matches at all
        return;

    typename std::multimap<const char *, ValueType>::iterator itr;
    itr = mmap.find(key); // start at first occurrence
    while (itr != mmap.upper_bound(key)) {
        if (itr->second == val) { // iterate to find matching pair
            mmap.erase(itr);
            break;
        }
        ++itr;
    }
}

template <class ValueType> ValueType Hashtable<ValueType>::find(const char *key) {
    ValueType found = NULL;

    if (mmap.count(key) > 0) {
        typename std::multimap<const char *, ValueType>::iterator cur, last, prev;
        cur = mmap.find(key); // start at first occurrence
        last = mmap.upper_bound(key);
        while (cur != last) { // iterate to find last entered
            prev = cur;
            if (++cur == mmap.upper_bound(key)) { // have to go one too far
                found = prev->second; // one before last was it
                break;
            }
        }
    }
    return found;
}

template <class ValueType>
int Hashtable<ValueType>::size() const {
    return mmap.size();
}

template <class ValueType> Iterator<ValueType> Hashtable<ValueType>::GetIterator() {
    return Iterator<ValueType>(mmap);
}

template <class ValueType> ValueType Iterator<ValueType>::next() {
    return (cur == end ? NULL : (*cur++).second);
}

#endif // HASHTABLE_H_
