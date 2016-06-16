#include "gamelib/sprite/SpriteSet.hpp"
#include "gamelib/utils/log.hpp"
#include <cassert>

namespace gamelib
{
    bool SpriteSet::loadFromJson(const Json::Value& node)
    {
        destroy();

        if (!node.isMember("sheet") || !_sheet.loadFromFile(node["sheet"].asString()))
        {
            LOG_ERROR("Failed to load sprite sheet");
            return false;
        }

        if (!node.isMember("sprites"))
        {
            LOG_WARN("No sprites defined");
            return true;
        }

        for (auto i = node["sprites"].begin(), end = node["sprites"].end(); i != end; ++i)
        {
            SpriteData data;
            data.anidata.speed = i->get("speed", 0).asFloat();
            data.anidata.length = i->get("length", 1).asInt();
            data.anidata.offset = i->get("offset", 0).asFloat();

            if (i->isMember("rect"))
            {
                const auto& rect = (*i)["rect"];
                data.anidata.rect.left = rect.get("x", 0).asInt();
                data.anidata.rect.top = rect.get("y", 0).asInt();
                data.anidata.rect.width = rect.get("w", 0).asInt();
                data.anidata.rect.height = rect.get("h", 0).asInt();
            }
            else
                LOG_WARN("No rect specified for \"", i.key().asString(), "\"");

            if (i->isMember("origin"))
            {
                auto& origin = (*i)["origin"];
                data.origin.x = origin.get("x", 0).asFloat();
                data.origin.y = origin.get("y", 0).asFloat();
            }

            add(i.key().asString(), data);
        }

        LOG_DEBUG("Loaded SpriteSet with ", _sprites.size(), " sprites");

        return true;
    }

    void SpriteSet::destroy()
    {
        _sprites.clear();
        _sheet = sf::Texture();
        LOG_DEBUG_WARN("SpriteSet destroyed");
    }


    void SpriteSet::add(const SpriteID& key, const SpriteData& spr)
    {
        _sprites[key] = spr;
    }

    void SpriteSet::add(SpriteID&& key, const SpriteData& spr)
    {
        _sprites[key] = spr;
    }

    void SpriteSet::setSpriteSheet(const sf::Texture& tex)
    {
        _sheet = tex;
    }


    sf::Sprite SpriteSet::getSprite(const SpriteID& key) const
    {
        return getSprite(_sprites.find(key));
    }

    AnimatedSprite SpriteSet::getAnimatedSprite(const SpriteID& key) const
    {
        return getAnimatedSprite(_sprites.find(key));
    }

    const SpriteData& SpriteSet::getSpriteData(const SpriteID& key) const
    {
        assert("Sprite does not exist" && _sprites.find(key) != _sprites.end());
        return _sprites.find(key)->second;
    }

    bool SpriteSet::hasSprite(const SpriteID& key) const
    {
        return _sprites.find(key) != _sprites.end();
    }


    sf::Sprite SpriteSet::getSprite(const const_iterator& it) const
    {
        assert("Sprite does not exist" && it != _sprites.end());
        return sf::Sprite(_sheet, it->second.anidata.getRect(_sheet.getSize().x, _sheet.getSize().y));
    }

    AnimatedSprite SpriteSet::getAnimatedSprite(const const_iterator& it) const
    {
        assert("Sprite does not exist" && it != _sprites.end());
        return AnimatedSprite(_sheet, it->second);
    }


    const sf::Texture& SpriteSet::getSpriteSheet() const
    {
        return _sheet;
    }

    size_t SpriteSet::size() const
    {
        return _sprites.size();
    }


    SpriteSet::iterator SpriteSet::begin()
    {
        return _sprites.begin();
    }

    SpriteSet::const_iterator SpriteSet::begin() const
    {
        return _sprites.begin();
    }


    SpriteSet::iterator SpriteSet::end()
    {
        return _sprites.end();
    }

    SpriteSet::const_iterator SpriteSet::end() const
    {
        return _sprites.end();
    }

    SpriteSet::iterator SpriteSet::find (const SpriteID& key)
    {
        return _sprites.find(key);
    }

    SpriteSet::const_iterator SpriteSet::find (const SpriteID& key) const
    {
        return _sprites.find(key);
    }
}
