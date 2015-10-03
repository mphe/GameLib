#include "tile/TileSet.hpp"
#include "utils/log.hpp"
#include <cassert>

namespace engine
{
    bool TileSet::loadFromJson(const Json::Value& node)
    {
        _tilesize.x = node.get("tilew", 0).asInt();
        _tilesize.y = node.get("tileh", 0).asInt();
        LOG_DEBUG(LOG_DUMP(_tilesize.x), ", ", LOG_DUMP(_tilesize.y));

        if (!_tilesize.x || !_tilesize.y)
        {
            LOG_ERROR("Invalid tile size");
            return false;
        }

        if (!node.isMember("tilesheet") || !_sheet.loadFromFile(node["tilesheet"].asString()))
        {
            LOG_ERROR("Failed to load tilesheet");
            return false;
        }

        if (!node.isMember("tiles"))
        {
            LOG_WARN("No tiles specified");
            return false;
        }

        const auto& tiles = node["tiles"];
        _tiles.resize(tiles.size());

        LOG_DEBUG("Loading tiles...");
        for (Json::ArrayIndex i = 0; i < tiles.size(); ++i)
        {
            const auto& t = tiles[i];
            auto& td = _tiles[i].texdata;

            LOG_DEBUG("Loading tile ", i, ":");
            if (t.isMember("name"))
            {
                _aliases[t["name"].asString()] = &_tiles[i];
                LOG_DEBUG("alias: ", t["name"].asString());
            }
            _tiles[i].id = i;
            td.index = t.get("index", 0).asInt();
            td.offset = t.get("offset", 0).asFloat();
            td.length = t.get("anilen", 1).asInt();
            td.speed = t.get("anispeed", 0).asFloat();

            LOG_DEBUG(LOG_DUMP(td.index));
            LOG_DEBUG(LOG_DUMP(td.offset));
            LOG_DEBUG(LOG_DUMP(td.length));
            LOG_DEBUG(LOG_DUMP(td.speed));
        }

        LOG("Tileset with ", _tiles.size(), " tiles loaded");
        return true;
    }

    void TileSet::destroy()
    {
        _tilesize.set(0);
        _sheet = sf::Texture();
        _tiles.clear();
        _aliases.clear();
        LOG_WARN("Tileset destroyed");
    }


    sf::Sprite TileSet::getSprite(TileID tileid, int offset) const
    {
        return sf::Sprite(_sheet, getTexRect(tileid, offset));
    }

    sf::Sprite TileSet::getSprite(int index) const
    {
        return sf::Sprite(_sheet, getTexRect(index));
    }

    sf::IntRect TileSet::getTexRect(TileID tileid, int offset) const
    {
        return getTexRect(_tiles[tileid].texdata.index + offset);
    }

    sf::IntRect TileSet::getTexRect(int index) const
    {
        return sf::IntRect( // left, top, width, height
            (index * _tilesize.x) % _sheet.getSize().x,
            ((index * _tilesize.x) / _sheet.getSize().x) * _tilesize.y,
            _tilesize.x,
            _tilesize.y
        );
    }


    bool TileSet::hasTile(TileID id) const
    {
        return id < _tiles.size();
    }

    bool TileSet::hasTile(const std::string& name) const
    {
        return _aliases.find(name) != _aliases.end();
    }


    const TileData& TileSet::getTile(TileID tileid) const
    {
        return _tiles[tileid];
    }

    const TileData& TileSet::getTile(const std::string& name) const
    {
        assert("Alias does not exist" && _aliases.count(name) > 0); 
        return *_aliases.at(name);
    }


    const sf::Texture& TileSet::getTexSheet() const
    {
        return _sheet;
    }

    const geometry::Vector2<int>& TileSet::getTileSize() const
    {
        return _tilesize;
    }

    size_t TileSet::size() const
    {
        return _tiles.size();
    }
}
