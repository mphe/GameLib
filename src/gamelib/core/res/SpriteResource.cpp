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

    auto SpriteResourceData::getFrameRect(int index) -> math::AABBi
    {
        // Adding this to the tex coords will prevent the 1px border glitch (hopefully)
        // https://gamedev.stackexchange.com/a/75244
        // https://stackoverflow.com/questions/19611745/opengl-black-lines-in-between-tiles
        // constexpr float magic = 0.375;
        constexpr float magic = 0;

        auto tsize = tex->getSize();

        int x = rect.x + index * rect.w;
        int y = (rect.y + (int)(x / tsize.x) * rect.h) % tsize.y;
        x = x % tsize.x;

        return math::AABBi(
                x + magic, y + magic,
                x + rect.w - magic, y + rect.h - magic);
    }

    BaseResourceHandle spriteLoader(const std::string& fname, ResourceManager* resmgr)
    {
        Json::Value node;
        if (!loadJsonFromFile(fname, node))
            return nullptr;

        auto sprite = SpriteResource::create();

        if (node.isMember("parent"))
        {
            auto parent = resmgr->get(node["parent"].asString()).as<SpriteResource>();
            if (!parent)
                LOG_ERROR("Invalid parent sprite");
            else
                *sprite = *parent;
        }

        if (node.isMember("texture"))
            sprite->tex = resmgr->get(node["texture"].asString()).as<TextureResource>();

        if (!sprite->tex)
        {
            LOG_ERROR("No texture specified for sprite: ", fname);
            return nullptr;
        }

        loadFromJson(node["framesize"], sprite->rect.size);

        if (sprite->rect.size.isZero())
            sprite->rect.size = convert(sprite->tex->getSize());

        if (node.isMember("startindex"))
            sprite->rect.pos = sprite->getFrameRect(node["startindex"].asInt()).pos;

        loadFromJson(node["framepos"], sprite->rect.pos);    // overwrites startindex if present
        loadFromJson(node["origin"], sprite->origin);

        sprite->ani.length = node.get("length", sprite->ani.length).asInt();
        sprite->ani.speed = node.get("speed", sprite->ani.speed).asFloat();
        sprite->ani.setIndex(node.get("offset", sprite->ani.offset).asInt());

        return sprite.as<BaseResource>();
    }
}
