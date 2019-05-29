#include "gamelib/json/json-rendering.hpp"
#include "gamelib/json/json-resources.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"

namespace gamelib
{
    void writeToJson(Json::Value& node, const RenderOptions& options)
    {
        // TODO: blendmode, texture, shader
        node["flags"] = options.flags;
        node["parallax"] = options.parallax;
        writeToJson(node["texture"], options.texture);
    }

    bool loadFromJson(const Json::Value& node, RenderOptions* options)
    {
        // TODO: blendmode, shader
        options->flags = node.get("flags", options->flags).asUInt();
        options->parallax = node.get("parallax", options->parallax).asFloat();

        if (node.isMember("texture"))
            loadFromJson(node["texture"], &options->texture);

        return true;
    }

    void writeToJson(Json::Value& node, const RenderLayer& layer)
    {
        writeToJson(node, layer.options);
        node["depth"] = layer.depth;
        node["name"] = layer.name;
    }

    bool loadFromJson(const Json::Value& node, RenderLayer* layer)
    {
        if (!node.isObject())
            return false;
        layer->name = node.get("name", layer->name).asString();
        layer->depth = node.get("depth", layer->depth).asInt();
        return loadFromJson(node, &layer->options);
    }
}
