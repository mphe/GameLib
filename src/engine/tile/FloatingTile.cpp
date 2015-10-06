#include "engine/tile/FloatingTile.hpp"
#include "engine/tile/TileSet.hpp"
#include "engine/utils/log.hpp"

namespace engine
{
    FloatingTile::FloatingTile(const TileSet& tileset, float x, float y) :
        FloatingTile(tileset, InvalidTile, x, y)
    { }

    FloatingTile::FloatingTile(const TileSet& tileset, TileID id, float x, float y) :
        Tile(tileset)
    {
        _sprite = _tileset.getSprite(id, 0);
        _change(id);
        _sprite.setPosition(x, y);
    }


    bool FloatingTile::loadFromJson(const Json::Value& node)
    {
        LOG_DEBUG("Loading FloatingTile from Json");
        float x = node.get("xt", 0).asInt() * _tileset.getTileSize().x,
              y = node.get("yt", 0).asInt() * _tileset.getTileSize().y;
        x += node.get("x", 0).asFloat();
        y += node.get("y", 0).asFloat();

        LOG_DEBUG(LOG_DUMP(x), ", ", LOG_DUMP(y));

        TileID id = node.isMember("tileid") ? _tileset.getTileID(node["tileid"].asString()) : InvalidTile;
        _change(id);
        _sprite.setPosition(x, y);

        LOG_DEBUG(LOG_DUMP(_tiledata.id));

        return true;
    }


    void FloatingTile::update(float fps)
    {
        Tile::update(fps);
        _sprite.setTextureRect(_getTexRect());
    }

    void FloatingTile::render(sf::RenderTarget& target) const
    {
        target.draw(_sprite);
    }

    void FloatingTile::_change(TileID id)
    {
        Tile::_change(id);
        _sprite.setTextureRect(_getTexRect());
    }
}
