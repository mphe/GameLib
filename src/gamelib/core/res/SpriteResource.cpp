#include "gamelib/core/res/SpriteResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    void registerSpriteLoader(ResourceManager& resmgr)
    {
        resmgr.registerFileType("spr", spriteLoader);
    }

    BaseResourceHandle spriteLoader(const std::string& fname, ResourceManager* resmgr)
    {
        Json::Value node;
        if (!loadJsonFromFile(fname, node))
            return nullptr;

        if (!node.isMember("texture"))
        {
            LOG_ERROR("No texture specified for sprite: ", fname);
            return nullptr;
        }

        auto sprite = SpriteResource::create();
        sprite->tex = resmgr->get(node["texture"].asString()).as<TextureResource>();

        math::Vec2f tmp;

        if (loadFromJson(node["framepos"], tmp))
            sprite->rect.pos = tmp;

        if (loadFromJson(node["framesize"], tmp))
            sprite->rect.size = tmp;

        sprite->ani.length = node.get("length", 1).asInt();
        sprite->ani.speed = node.get("speed", 0).asFloat();
        sprite->ani.setIndex(node.get("offset", 0).asInt());

        return sprite.as<BaseResource>();
    }
}
