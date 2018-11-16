#include "gamelib/utils/json.hpp"
#include "gamelib/utils/conversions.hpp"
#include "gamelib/core/geometry/GroupTransform.hpp"
#include "gamelib/core/geometry/Transformable.hpp"
#include <fstream>

namespace gamelib
{
    bool diffJson(const Json::Value& node, const Json::Value& compare, Json::Value* out_)
    {
        assert(out_ && "Destination node is null");
        auto& out = *out_;

        if (node.isNumeric() && compare.isNumeric())
        {
            if (node.isIntegral() && compare.isIntegral() && node.asInt() == compare.asInt())
                return false;
            else if (math::almostEquals(node.asDouble(), compare.asDouble(), 0.00001))
                return false;
            else if (math::almostEquals(node.asDouble(), compare.asDouble(), 0.0001))
                LOG_DEBUG_WARN("A double value differs with tolerance 0.0001 but not ", 0.00001);
        }
        else if (node.type() == compare.type())
        {
            if (node.isObject())
            {
                bool diff = false;
                for (auto it = node.begin(), end = node.end(); it != end; ++it)
                {
                    auto key = it.key().asString();
                    if (!compare.isMember(key))
                    {
                        out[key] = *it;
                        diff = true;
                    }
                    else
                    {
                        Json::Value val;
                        if (diffJson(*it, compare[key], &val))
                        {
                            out[key] = val;
                            diff = true;
                        }
                    }
                }
                return diff;
            }
            else if (node.isArray())
            {
                if (node.size() == compare.size())
                {
                    for (Json::ArrayIndex i = 0; i < node.size(); ++i)
                    {
                        Json::Value tmp;
                        if (diffJson(node[i], compare[i], &tmp))
                        {
                            out = node;
                            return true;
                        }
                    }
                    return false;
                }
            }
            else if (node.compare(compare) == 0)
                return false;
        }

        out = node;
        return true;
    }

    void mergeJson(const Json::Value& src, Json::Value* dest)
    {
        assert(dest && "Destination node is null");

        for (auto it = src.begin(), end = src.end(); it != end; ++it)
        {
            auto& out = (*dest)[it.key().asString()];

            if (it->isObject())
                mergeJson(*it, &out);
            else
                out = *it;
        }
    }


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


    bool loadFromJson(const Json::Value& node, math::Point2f& p, bool clear)
    {
        return loadFromJson(node, p.asVector(), clear);
    }

    bool loadFromJson(const Json::Value& node, sf::Vector2f& p, bool clear)
    {
        return loadFromJson(node, convert(p), clear);
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


    void writeToJson(Json::Value& node, const math::Point2f& p)
    {
        writeToJson(node, p.asVector());
    }

    void writeToJson(Json::Value& node, const sf::Vector2f& vec)
    {
        writeToJson(node, convert(vec));
    }
}
