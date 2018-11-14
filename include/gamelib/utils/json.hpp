#ifndef GAMELIB_JSON_UTILS_HPP
#define GAMELIB_JSON_UTILS_HPP

#include <cassert>
#include <SFML/Graphics.hpp>
#include "json/json.h"
#include "math/geometry/Vector.hpp"
#include "gamelib/utils/log.hpp"

/*
 * Various functions for writing and loading data to/from json.
 * Default values are always the given values: if there's no entry in the
 * json, the corresponding value will not be changed.
 */

namespace gamelib
{
    class Transformable;
    class TransformData;

    // Compares node against compare and writes everything non-existent or different in compare to out.
    bool diffJson(const Json::Value& node, const Json::Value& compare, Json::Value* out_);

    bool loadJsonFromFile(const std::string& fname, Json::Value& node);
    bool writeJsonToFile(const std::string& fname, const Json::Value& node);

    bool loadFromJson(const Json::Value& node, Transformable& trans, bool clear);
    bool loadFromJson(const Json::Value& node, TransformData& data, bool clear);

    // TODO: implement Vec2i serialization
    bool loadFromJson(const Json::Value& node, math::Point2f& p, bool clear = false);
    bool loadFromJson(const Json::Value& node, sf::Vector2f& vec, bool clear = false);

    void writeToJson(Json::Value& node, const Transformable& trans);
    void writeToJson(Json::Value& node, const TransformData& data);

    void writeToJson(Json::Value& node, const math::Point2f& p);
    void writeToJson(Json::Value& node, const sf::Vector2f& vec);


    template <typename T, size_t N>
    bool loadFromJson(const Json::Value& node, math::Vector<T, N>& vec, bool clear = false);

    template <typename T, size_t N>
    void writeToJson(Json::Value& node, const math::Vector<T, N>& vec)
    {
        node.resize(N);
        for (Json::ArrayIndex i = 0; i < N; ++i)
            node[i] = vec[i];
    }


    template <typename T>
    void writeToFile(const std::string& fname, const T& obj)
    {
        Json::Value json;
        writeToJson(json, obj);
        writeJsonToFile(fname, json);
    }

    template <typename T>
    bool loadFromFile(const std::string& fname, T& obj)
    {
        Json::Value json;
        if (!loadJsonFromFile(fname, json))
            return false;
        return loadFromJson(json, obj);
    }


#define _LOAD_BASIC_OVERLOAD(type, name) \
    inline bool loadFromJsonBasic(const Json::Value& node, type& obj)  \
    {   \
        obj = node.as##name(); \
        return true;    \
    }

    _LOAD_BASIC_OVERLOAD(int, Int)
    _LOAD_BASIC_OVERLOAD(unsigned int, UInt)
    _LOAD_BASIC_OVERLOAD(bool, Bool)
    _LOAD_BASIC_OVERLOAD(float, Float)
    _LOAD_BASIC_OVERLOAD(double, Double)
    _LOAD_BASIC_OVERLOAD(std::string, String)

#undef _LOAD_BASIC_OVERLOAD

    template <typename T, size_t N>
    bool loadFromJson(const Json::Value& node, math::Vector<T, N>& vec, bool clear)
    {
        if (!node.isArray())
            return false;
        if (node.size() != N)
            LOG_WARN("Array size doesn't match expected size: ", node.size(), " != ", N);
        for (Json::ArrayIndex i = 0; i < std::min((size_t)node.size(), N); ++i)
            loadFromJsonBasic(node.get(i, clear ? 0 : vec[i]), vec[i]);
        return true;
    }

}

#endif
