#ifndef GAMELIB_JSON_UTILS_HPP
#define GAMELIB_JSON_UTILS_HPP

#include <SFML/Graphics.hpp>
#include "json/json.h"
#include "math/geometry/Vector.hpp"

namespace gamelib
{
    class Transformable;

    bool loadJsonFromFile(const std::string& fname, Json::Value& node);

    bool loadFromJson(const Json::Value& node, Transformable& trans);
    bool loadFromJson(const Json::Value& node, math::Vec2f& vec);
    bool loadFromJson(const Json::Value& node, math::Point2f& p);
    bool loadFromJson(const Json::Value& node, sf::Vector2f& vec);

    void writeToJson(Json::Value& node, const Transformable& trans);
    void writeToJson(Json::Value& node, const math::Vec2f& vec);
    void writeToJson(Json::Value& node, const math::Point2f& p);
    void writeToJson(Json::Value& node, const sf::Vector2f& vec);
}

#endif
