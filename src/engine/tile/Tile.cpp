#include "engine/tile/Tile.hpp"

namespace engine
{
    void Tile::update(float fps)
    {
        auto& td = _tiledata.texdata;
        td.offset = std::fmod(td.offset + td.speed / fps, td.length);
    }

    const TileData& Tile::getTileData() const
    {
        return _tiledata;
    }
}
