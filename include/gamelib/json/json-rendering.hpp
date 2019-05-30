#ifndef GAMELIB_JSON_RENDERING_HPP
#define GAMELIB_JSON_RENDERING_HPP

#include "json/json.h"

namespace gamelib
{
    class RenderOptions;
    class RenderLayer;
    class RenderNode;
    class RenderSystem;

    void writeToJson(Json::Value& node, const RenderOptions& options);
    bool loadFromJson(const Json::Value& node, RenderOptions* options);

    void writeToJson(Json::Value& node, const RenderLayer& layer);
    bool loadFromJson(const Json::Value& node, RenderLayer* layer);
}

#endif
