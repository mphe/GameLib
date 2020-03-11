#ifndef GAMELIB_JSON_TRANSFORMABLE_HPP
#define GAMELIB_JSON_TRANSFORMABLE_HPP

#include "json/json.h"

namespace gamelib
{
    class Transformable;
    class TransformData;

    bool loadFromJson(const Json::Value& node, Transformable& trans, bool clear, bool global = false);
    bool loadFromJson(const Json::Value& node, TransformData& data, bool clear);

    void writeToJson(Json::Value& node, const Transformable& trans, bool global = false);
    void writeToJson(Json::Value& node, const TransformData& data);
}

#endif
