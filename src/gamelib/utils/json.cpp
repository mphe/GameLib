#include "gamelib/utils/json.hpp"
#include "gamelib/geometry/Transformable.hpp"
#include <fstream>

namespace gamelib
{
    bool loadJsonFromFile(const std::string& fname, Json::Value& node)
    {
        std::ifstream f;
        f.open(fname.c_str());
        if (f.is_open())
        {
            f >> node;
            f.close();
            return true;
        }
        return false;
    }

    bool loadFromJson(const Json::Value& node, Transformable& transform)
    {
        if (!node.isObject())
            return false;
        math::Point2f pos;
        math::Vec2f scale;
        loadFromJson(node["pos"], pos);
        loadFromJson(node["scale"], scale);
        transform.setPosition(pos);
        transform.setScale(scale);
        transform.setRotation(node.get("angle", 0).asFloat());
        return true;
    }

    bool loadFromJson(const Json::Value& node, math::Vec2f& vec)
    {
        if (!node.isArray())
            return false;
        vec.x = node.get((Json::ArrayIndex)0, 0).asFloat();
        vec.y = node.get(1, 0).asFloat();
        return true;
    }

    bool loadFromJson(const Json::Value& node, math::Point2f& p)
    {
        if (!node.isArray())
            return false;
        p.x = node.get((Json::ArrayIndex)0, 0).asFloat();
        p.y = node.get(1, 0).asFloat();
        return true;
    }


    void writeToJson(Json::Value& node, const Transformable& transform)
    {
        writeToJson(node["pos"], transform.getPosition());
        writeToJson(node["scale"], transform.getScale());
        node["angle"] = transform.getRotation();
    }

    void writeToJson(Json::Value& node, const math::Vec2f& vec)
    {
        node.resize(2);
        node[0] = vec.x;
        node[1] = vec.y;
        // node["x"] = vec.x;
        // node["y"] = vec.y;
    }

    void writeToJson(Json::Value& node, const math::Point2f& p)
    {
        node.resize(2);
        node[0] = p.x;
        node[1] = p.y;
        // node["x"] = p.x;
        // node["y"] = p.y;
    }
}
