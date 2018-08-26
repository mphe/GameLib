#ifndef GAMELIB_JSON_UTILS_HPP
#define GAMELIB_JSON_UTILS_HPP

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

    // Compares node against compare and writes everything non-existent or different in compare to out.
    bool diffJson(const Json::Value& node, const Json::Value& compare, Json::Value* out_);

    bool loadJsonFromFile(const std::string& fname, Json::Value& node);
    bool writeJsonToFile(const std::string& fname, const Json::Value& node);

    bool loadFromJson(const Json::Value& node, Transformable& trans);
    bool loadFromJson(const Json::Value& node, math::Point2f* pos, math::Vec2f* scale, float* angle, bool clear);

    // TODO: implement Vec2i serialization
    bool loadFromJson(const Json::Value& node, math::Point2f& p, bool clear = false);
    bool loadFromJson(const Json::Value& node, sf::Vector2f& vec, bool clear = false);

    void writeToJson(Json::Value& node, const Transformable& trans);
    void writeToJson(Json::Value& node, const math::Point2f& pos, const math::Vec2f& scale, float angle);

    void writeToJson(Json::Value& node, const math::Point2f& p);
    void writeToJson(Json::Value& node, const sf::Vector2f& vec);


    template <size_t N>
    bool loadFromJson(const Json::Value& node, math::Vector<float, N>& vec, bool clear = false)
    {
        if (!node.isArray())
            return false;
        if (node.size() != N)
            LOG_WARN("Array size doesn't match expected size: ", node.size(), " != ", N);
        for (Json::ArrayIndex i = 0; i < std::min((size_t)node.size(), N); ++i)
            vec[i] = node.get(i, clear ? 0 : vec[i]).asFloat();
        return true;
    }

    template <size_t N>
    void writeToJson(Json::Value& node, const math::Vector<float, N>& vec)
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
}

#endif
