#include "tile/FloatingTile.hpp"
#include "tile/TileSet.hpp"

namespace engine
{
    FloatingTile::FloatingTile(const TileSet& tileset, const Json::Value& params) :
        _tileset(tileset)
    {
        _sprite = _tileset.getSprite(InvalidTile);
        loadFromJson(params);
    }

    FloatingTile::FloatingTile(const TileSet& tileset, TileID id, float x, float y) :
        _tileset(tileset)
    {
        _sprite = _tileset.getSprite(InvalidTile);
        _change(id);
        _sprite.setPosition(x, y);
    }


    bool FloatingTile::loadFromJson(const Json::Value& node)
    {
        float x = node.get("xt", 0).asInt() * _tileset.getTileSize().x,
              y = node.get("yt", 0).asInt() * _tileset.getTileSize().y;
        x += node.get("x", 0).asFloat();
        y += node.get("y", 0).asFloat();

        _change(node.get("tileid", InvalidTile).asInt());
        _sprite.setPosition(x, y);

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
        _tiledata = _tileset.getTile(id);
        _sprite.setTextureRect(_getTexRect());
    }

    sf::IntRect FloatingTile::_getTexRect() const
    {
        return _tileset.getTexRect(_tiledata.id, _tiledata.texdata.offset);
    }
}
