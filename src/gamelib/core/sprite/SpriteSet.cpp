#include "gamelib/core/sprite/SpriteSet.hpp"
#include "gamelib/utils/log.hpp"
#include <cassert>

namespace gamelib
{
    bool SpriteSet::_loadSprite(const Json::Value& node, SpriteData& data)
    {
        data.anidata.speed = node.get("speed", 0).asFloat();
        data.anidata.length = node.get("length", 1).asInt();
        data.anidata.offset = node.get("offset", 0).asFloat();

        if (node.isMember("rect"))
        {
            const auto& rect = node["rect"];
            data.anidata.rect.left = rect.get("x", 0).asInt();
            data.anidata.rect.top = rect.get("y", 0).asInt();
            data.anidata.rect.width = rect.get("w", 0).asInt();
            data.anidata.rect.height = rect.get("h", 0).asInt();
        }
        else
        {
            LOG_ERROR("No rect specified");
            return false;
        }

        if (node.isMember("origin"))
        {
            auto& origin = node["origin"];
            data.origin.x = origin.get("x", 0).asFloat();
            data.origin.y = origin.get("y", 0).asFloat();
        }

        return true;
    }

    bool SpriteSet::loadFromJson(const Json::Value& node)
    {
        if (!node.isMember("sheet") || !_sheet.loadFromFile(node["sheet"].asString()))
        {
            LOG_ERROR("Failed to load sprite sheet");
            return false;
        }

        return JsonDataSet<SpriteData>::loadFromJson(node);
    }

    void SpriteSet::destroy()
    {
        _sheet = sf::Texture();
        JsonDataSet<SpriteData>::destroy();
    }

    void SpriteSet::setSpriteSheet(const sf::Texture& tex)
    {
        _sheet = tex;
    }

    const sf::Texture& SpriteSet::getSpriteSheet() const
    {
        return _sheet;
    }


    sf::Sprite SpriteSet::getSprite(const DataSetKey& key) const
    {
        return getSprite(find(key));
    }

    sf::Sprite SpriteSet::getSprite(const const_iterator& it) const
    {
        assert("Sprite does not exist" && it != end());
        return sf::Sprite(_sheet, it->second.anidata.getRect(
                    _sheet.getSize().x, _sheet.getSize().y));
    }

    AnimatedSprite SpriteSet::getAnimatedSprite(const DataSetKey& key) const
    {
        return getAnimatedSprite(find(key));
    }

    AnimatedSprite SpriteSet::getAnimatedSprite(const const_iterator& it) const
    {
        assert("Sprite does not exist" && it != end());
        return AnimatedSprite(_sheet, it->second);
    }
}
