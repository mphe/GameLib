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
        vec.x = node.get("x", 0).asFloat();
        vec.y = node.get("y", 0).asFloat();
        return true;
    }

    bool loadFromJson(const Json::Value& node, math::Point2f& p)
    {
        p.x = node.get("x", 0).asFloat();
        p.y = node.get("y", 0).asFloat();
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
        node["x"] = vec.x;
        node["y"] = vec.y;
    }

    void writeToJson(Json::Value& node, const math::Point2f& p)
    {
        node["x"] = p.x;
        node["y"] = p.y;
    }
}
