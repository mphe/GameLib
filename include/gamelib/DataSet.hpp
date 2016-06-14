#ifndef GAMELIB_DATASET_HPP
#define GAMELIB_DATASET_HPP

#include <string>
#include <unordered_map>

/*
 * A collection to store a set of data (duh).
 * There's currently no custom implementation because a custom implementation
 * would mostly be a wrapper around std::unordered_map.
 * Instead of directly using std::unordered_map wherever a data set is required,
 * this DataSet alias should be used instead to make it easier to switch to
 * a custom implementation later (if there will ever be one).
 *
 * A DataSet must be std map-like, that means it must...:
 *  - be iterable (std compatible) and therefore...
 *  - have begin() and end() functions that return an (const) iterator to a
 *    std::pair containing the key and value.
 *  - have a size() function
 *  - have a find() function that takes a key as parameter and returns
 *    an (const) iterator to the key-value pair and end() if it doesn't exist.
 */

namespace gamelib
{
    typedef std::string DataSetKey;

    template <class T>
    using DataSet = std::unordered_map<DataSetKey, T>;

    // template <class T>
    // class DataSet : private std::unordered_map<DataSetKey, T>
    // {
    //     typedef std::unordered_map<DataSetKey, T> Map;
    //
    //     public:
    //         using Map::find;
    //         using Map::size;
    //         using Map::operator[];
    //         using Map::begin;
    //         using Map::end;
    // };
}

#endif
