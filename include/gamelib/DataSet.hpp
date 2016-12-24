#ifndef GAMELIB_DATASET_HPP
#define GAMELIB_DATASET_HPP

#include <string>
#include <unordered_map>
#include "utils/JsonObject.hpp"
#include "utils/log.hpp"

/*
 * TODO: Find out if this bullshit is REALLY necessary. Reading it now, long
 * after I wrote it, gives me headaches.
 *
 * A collection to store a set of data (duh).
 *
 * This is the implementation for a basic dataset and should work in most
 * cases. It's mostly a wrapper around std::unordered_map (using private
 * inheritance) and optionally extends it with functions to load from json
 * (by inheriting from JsonObject).
 * See further down for more information to the different DataSet types.
 *
 * Classes that require a dataset, for example StaticDataSetTilemap, should
 * take a template argument as dataset type rather than using DataSet
 * directly. This is because there might be different kinds of datasets,
 * e.g. SpriteSet, and forcing a specific type would lead to unneccessary
 * inheritance trouble/overhead or code repetition when a derived class
 * needs a specific type of dataset.
 *
 * For example, StaticRenderTilemap requires a SpriteSet. Using DataSet
 * directly in the underlying StaticDataSetTilemap instead of specifying
 * the type as template argument, would make it (more) difficult to use it
 * as a base for StaticRenderTilemap, because it would require rewriting
 * all dataset related functions to only allow SpriteSets.
 *
 * To avoid different interpretations of a dataset, these are the
 * requirements for a dataset class:
 *
 * A DataSet must be std map-like, that means it must...:
 *  - be iterable (std compatible) and therefore...
 *  - have begin() and end() functions that return an (const) iterator to a
 *    std::pair containing the key and value.
 *  - have a size() function
 *  - have a find() function that takes a key as parameter and returns
 *    an (const) iterator to the key-value pair and end() if it doesn't exist.
 *  - have a [] operator, taking a key and returning a reference to the
 *    object stored at the given key. If it doesn't exist, it is created.
 */

namespace gamelib
{
    typedef std::string DataSetKey;

    template <class T>
    using DataSetCallback = bool(*)(const Json::Value&, T&);

    /*
     * The base class for datasets.
     * It's basically a wrapper around std::unordered_map.
     */
    template <class T>
    class DataSet : protected std::unordered_map<DataSetKey, T>
    {
        typedef std::unordered_map<DataSetKey, T> Map;

        public:
            using Map::find;
            using Map::size;
            using Map::operator[];
            using Map::begin;
            using Map::end;

            using typename Map::iterator;
            using typename Map::const_iterator;
            using typename Map::difference_type;
            using typename Map::size_type;
            using typename Map::value_type;
            using typename Map::key_type;
            using typename Map::pointer;
            using typename Map::reference;

        public:
            virtual ~DataSet() {}
            virtual void destroy()
            {
                this->clear();
                LOG_DEBUG_WARN("DataSet destroyed");
            }
    };

    /*
     * A DataSet with json functionality.
     * It takes a type T and a function F as template arguments. T is the
     * data type to store and F is a function that takes a Json::Value aswell
     * as a T reference as parameters and returns a bool.
     * When loadFromJson is called, it will iterate trough all key-value pairs
     * inside the "objects" node and invoke F with the current value. Its job
     * is to fill the given T& with the corresponding values from the json
     * node and return whether it was successful or not.
     * In case F returns false, the value is not added and a warning message
     * is printed.
     *
     * Json structure:
     * Note that only values inside "objects" are read!
     * <value> will be passed to the callback function and can therefore be
     * any json type (array, dict, string, int, ...).
     * {
     *     "objects": {
     *         <objectname>: <value>,
     *         ...
     *     }
     * }
     */

    template <class T>
    class JsonDataSet : public DataSet<T>, public Json::JsonObject
    {
        public:
            JsonDataSet(DataSetCallback<T> callback) : _callback(callback) {}
            virtual ~JsonDataSet() {}

            virtual bool loadFromJson(const Json::Value& node)
            {
                this->clear();

                if (!node.isMember("objects"))
                {
                    LOG_ERROR("No objects specified");
                    return false;
                }

                auto& objs = node["objects"];
                for (auto i = objs.begin(); i != objs.end(); ++i)
                {
                    T o;
                    if (_callback(*i, o))
                        this->operator[](i.key().asString()) = o;
                    else
                        LOG_WARN("Failed to load '", i.key().asString(), "'");
                }

                LOG_DEBUG("DataSet loaded with ", this->size(), " objects");

                return true;
            }

        private:
            DataSetCallback<T> _callback;
    };

    /*
     * In some cases it might be useful to put different datasets in one file
     * to avoid having to write the exact same config multiple times.
     * To seperate the different datasets, "namespaces" can be used.
     *
     * The following function (namespaceReader) or alias (NamespaceDataSet)
     * can be used to read from a file using the namespace pattern.
     *
     * Json structure:
     * {
     *     "objects": {
     *         <objectname>: {
     *             <namespace>: <value>,
     *             ...
     *         },
     *         ...
     *     }
     * }
     *
     * Note that it's not possible to use string literals as template
     * arguments. Therefore you need to pass single chars (which is ugly but
     * it works), e.g.:
     * JsonDataSet<SomeType>(namespaceReader<SomeType, somefunc,
     *                    'n', 'a', 'm', 'e', 's', 'p', 'a', 'c', 'e'>)
     */

    template <class T, DataSetCallback<T> F, char... chrs>
    bool namespaceReader(const Json::Value& node, T& obj)
    {
        static const char str[] = { chrs..., 0 };
        if (node.isMember(str))
            return F(node[str], obj);
        return false;
    }


    // template <class T, DataSetCallback<T> F, const char* str>
    // bool namespaceReader(const Json::Value& node, T& obj)
    // {
    //     if (node.isMember(str))
    //         return F(node[str], obj);
    //     return false;
    // }
}

#endif
