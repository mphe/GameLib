#include "gamelib/utils/json.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/core/geometry/Transformable.hpp"
#include <fstream>

namespace gamelib
{
    bool loadJsonFromFile(const std::string& fname, Json::Value& node)
    {
        LOG("(Re)Loading ", fname, "...");
        std::ifstream f;
        f.open(fname.c_str());
        if (f.is_open())
        {
            f >> node;
            f.close();
            return true;
        }
        LOG_ERROR("Failed to load json from file ", fname);
        return false;
    }

    bool writeJsonToFile(const std::string& fname, const Json::Value& node)
    {
        LOG("Writing to file ", fname, "...");
        std::ofstream f;
        f.open(fname.c_str());
        if (f.is_open())
        {
            f << node.toStyledString();
            f.close();
            return true;
        }
        LOG_ERROR("Failed to write json to file ", fname);
        return false;
    }


    bool loadFromJson(const Json::Value& node, Transformable& transform)
    {
        if (!node.isObject())
            return false;
        math::Point2f pos;
        math::Vec2f scale(1, 1);
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
        vec.x = node.get((Json::ArrayIndex)0, vec.x).asFloat();
        vec.y = node.get(1, vec.y).asFloat();
        return true;
    }

    bool loadFromJson(const Json::Value& node, math::Point2f& p)
    {
        return loadFromJson(node, p.asVector());
    }

    bool loadFromJson(const Json::Value& node, sf::Vector2f& p)
    {
        math::Vec2f vec(p.x, p.y);
        bool res = loadFromJson(node, vec);
        p = sf::Vector2f(vec.x, vec.y);
        return res;
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
    }

    void writeToJson(Json::Value& node, const math::Point2f& p)
    {
        writeToJson(node, p.asVector());
    }

    void writeToJson(Json::Value& node, const sf::Vector2f& vec)
    {
        writeToJson(node, math::Vec2f(vec.x, vec.y));
    }
}
