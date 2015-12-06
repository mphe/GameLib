#include "engine/SpriteSet.hpp"
#include "engine/utils/log.hpp"
#include <cassert>

namespace engine
{

    bool SpriteSet::loadFromJson(const Json::Value& node)
    {
        if (!node.isMember("sheet") || !_sheet.loadFromFile(node["sheet"].asString()))
        {
            LOG_ERROR("Failed to load sprite sheet");
            return false;
        }

        for (auto i = node.begin(); i != node.end(); ++i)
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

            add(i.key().asString(), data);
        }

        LOG_DEBUG("Loaded SpriteSet with ", _sprites.size(), " sprites");

        return true;
    }

    void SpriteSet::destroy()
    {
        _sprites.clear();
        _aliases.clear();
        _sheet = sf::Texture();
        LOG_DEBUG_WARN("SpriteSet destroyed");
    }


    SpriteID SpriteSet::add(const std::string& name, const SpriteData& sprdata)
    {
        _sprites.push_back(sprdata);
        _sprites.back().id = _sprites.size() - 1;
        _aliases[name] = &_sprites.back();
        return _sprites.size() - 1;
    }

    void SpriteSet::setSpriteSheet(const sf::Texture& tex)
    {
        _sheet = tex;
    }


    sf::Sprite SpriteSet::getSprite(const std::string& name, int offset) const
    {
        assert("Sprite does not exist" && _aliases.find(name) != _aliases.end());
        return sf::Sprite(_sheet, getTexRect(_aliases.find(name)->second->id, offset));
    }

    sf::Sprite SpriteSet::getSprite(SpriteID id, int offset) const
    {
        return sf::Sprite(_sheet, getTexRect(id, offset));
    }


    sf::IntRect SpriteSet::getTexRect(const std::string& name, int offset) const
    {
        assert("Sprite does not exist" && _aliases.find(name) != _aliases.end());
        return getTexRect(_aliases.find(name)->second->id, offset);
    }

    sf::IntRect SpriteSet::getTexRect(SpriteID id, int offset) const
    {
        const sf::IntRect& rect = _sprites[id].rect;
        return sf::IntRect( // left, top, width, height
            (rect.left + offset * rect.width) % _sheet.getSize().x,
            rect.top + ((offset * rect.width) / _sheet.getSize().x) * rect.height,
            rect.width,
            rect.height
        );
    }


    const SpriteData& SpriteSet::getSpriteData(const std::string& name) const
    {
        return *_aliases.find(name)->second;
    }

    const SpriteData& SpriteSet::getSpriteData(SpriteID id) const
    {
        return _sprites[id];
    }


    const sf::Texture& SpriteSet::getTexSheet() const
    {
        return _sheet;
    }

    size_t SpriteSet::size() const
    {
        return _sprites.size();
    }

}
