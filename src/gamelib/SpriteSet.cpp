#include "gamelib/SpriteSet.hpp"
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
            data.speed = i->get("speed", 0).asFloat();
            data.length = i->get("length", 1).asInt();
            data.offset = i->get("offset", 0).asFloat();

            if (i->isMember("rect"))
            {
                const auto& rect = (*i)["rect"];
                data.rect.left = rect.get("x", 0).asInt();
                data.rect.top = rect.get("y", 0).asInt();
                data.rect.width = rect.get("w", 0).asInt();
                data.rect.height = rect.get("h", 0).asInt();
            }
            else
                LOG_WARN("No rect specified for \"", i.key().asString(), "\"");

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
        _sprites.insert(std::make_pair(key, spr));
    }

    void SpriteSet::setSpriteSheet(const sf::Texture& tex)
    {
        _sheet = tex;
    }


    sf::Sprite SpriteSet::getSFMLSprite(const SpriteID& key, int offset) const
    {
        assert("Sprite does not exist" && _sprites.find(key) != _sprites.end());
        return sf::Sprite(_sheet, getTexRect(key, offset));
    }

    sf::IntRect SpriteSet::getTexRect(const SpriteID& key, int offset) const
    {
        assert("Sprite does not exist" && _sprites.find(key) != _sprites.end());
        sf::IntRect rect = _sprites.find(key)->second.rect;
        rect.left = (rect.left + offset * rect.width) % _sheet.getSize().x;
        rect.top += ((offset * rect.width) / _sheet.getSize().x) * rect.height;
        return rect;
    }

    const Sprite SpriteSet::getSprite(const SpriteID& key) const
    {
        Sprite spr(_sheet, getTexRect(key));
        auto& data = _sprites.find(key)->second;
        spr.length = data.length;
        spr.speed = data.speed;
        spr.offset = data.offset;
        spr.startx = data.rect.left;
        spr.starty = data.rect.top;
        return spr;
    }

    const sf::Texture& SpriteSet::getSpriteSheet() const
    {
        return _sheet;
    }

    size_t SpriteSet::size() const
    {
        return _sprites.size();
    }

}
