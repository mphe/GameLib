#include "gamelib/json/json-transformable.hpp"
#include "gamelib/json/json-vector.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/core/geometry/Transformable.hpp"

namespace gamelib
{
    bool loadFromJson(const Json::Value& node, Transformable& transform, bool clear)
    {
        TransformData data = transform.getLocalTransformation();
        bool success = loadFromJson(node, data, clear);
        transform.setLocalTransformation(data);
        return success;
    }

    bool loadFromJson(const Json::Value& node, TransformData& data, bool clear)
    {
        if (clear)
            data.reset();

        if (!node.isObject())
            return clear;

        loadFromJson(node["pos"], data.pos);
        loadFromJson(node["scale"], data.scale);
        loadFromJson(node["origin"], data.origin);
        data.angle = node.get("angle", data.angle).asFloat();

        return true;
    }

    void writeToJson(Json::Value& node, const Transformable& transform)
    {
        writeToJson(node, transform.getLocalTransformation());
    }

    void writeToJson(Json::Value& node, const TransformData& data)
    {
        writeToJson(node["pos"], data.pos);
        writeToJson(node["scale"], data.scale);
        writeToJson(node["origin"], data.origin);
        node["angle"] = data.angle;
    }
}
