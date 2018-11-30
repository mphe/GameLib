#include "gamelib/core/res/SpriteResource.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/conversions.hpp"
#include "gamelib/json/json-vector.hpp"
#include "gamelib/json/json-file.hpp"

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

        math::Vec2f pos, size = convert(sprite->tex->getSize());
        loadFromJson(node["framepos"], pos);
        loadFromJson(node["framesize"], size);

        sprite->rect.pos = pos;
        sprite->rect.size = size;

        loadFromJson(node["origin"], sprite->origin);

        sprite->ani.length = node.get("length", 1).asInt();
        sprite->ani.speed = node.get("speed", 0).asFloat();
        sprite->ani.setIndex(node.get("offset", 0).asInt());

        return sprite.as<BaseResource>();
    }
}
