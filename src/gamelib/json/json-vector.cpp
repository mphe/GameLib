#include "gamelib/json/json-vector.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
    bool loadFromJson(const Json::Value& node, math::Point2f& p, bool clear)
    {
        return loadFromJson(node, p.asVector(), clear);
    }

    void writeToJson(Json::Value& node, const math::Point2f& p)
    {
        writeToJson(node, p.asVector());
    }

    bool loadFromJson(const Json::Value& node, sf::Vector2f& p, bool clear)
    {
        return loadFromJson(node, convert(p), clear);
    }

    void writeToJson(Json::Value& node, const sf::Vector2f& vec)
    {
        writeToJson(node, convert(vec));
    }
}
