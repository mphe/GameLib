#include "engine/tile/Tile.hpp"
#include "engine/tile/TileSet.hpp"

namespace engine
{
    Tile::Tile(const TileSet& tileset, TileID id) : _tileset(tileset)
    {
        _change(id);
    }
  
    void Tile::update(float fps)
    {
        auto& td = _tiledata.texdata;
        td.offset = std::fmod(td.offset + td.speed / fps, td.length);
    }

    const TileData& Tile::getTileData() const
    {
        return _tiledata;
    }

    void Tile::_change(TileID id)
    {
        _tiledata = _tileset.getTile(id);
    }

    sf::IntRect Tile::_getTexRect() const
    {
        return _tileset.getTexRect(_tiledata.id, _tiledata.texdata.offset);
    }
}
