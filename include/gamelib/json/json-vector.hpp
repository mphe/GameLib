#ifndef GAMELIB_JSON_VECTOR_HPP
#define GAMELIB_JSON_VECTOR_HPP

#include "gamelib/utils/log.hpp"
#include "math/geometry/Vector.hpp"
#include "gamelib/json/json-natives.hpp"
#include <SFML/System/Vector2.hpp>

namespace gamelib
{
    // TODO: write as template
    bool loadFromJson(const Json::Value& node, math::Point2f& p, bool clear = false);
    void writeToJson(Json::Value& node, const math::Point2f& p);

    bool loadFromJson(const Json::Value& node, sf::Vector2f& vec, bool clear = false);
    void writeToJson(Json::Value& node, const sf::Vector2f& vec);

    template <typename T, size_t N>
    bool loadFromJson(const Json::Value& node, math::Vector<T, N>& vec, bool clear = false)
    {
        if (!node.isArray())
            return false;
        if (node.size() != N)
            LOG_WARN("Array size doesn't match expected size: ", node.size(), " != ", N);
        for (Json::ArrayIndex i = 0; i < std::min((size_t)node.size(), N); ++i)
            loadFromJson(node.get(i, clear ? 0 : vec[i]), vec[i]);
        return true;
    }

    template <typename T, size_t N>
    void writeToJson(Json::Value& node, const math::Vector<T, N>& vec)
    {
        node.resize(N);
        for (Json::ArrayIndex i = 0; i < N; ++i)
            node[i] = vec[i];
    }
}

#endif
