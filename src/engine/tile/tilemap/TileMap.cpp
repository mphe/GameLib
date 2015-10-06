#include "engine/tile/tilemap/TileMap.hpp"

namespace engine
{
    TileMap::TileMap() :
        _tilesRendered(0),
        _layer(0)
    { }

    int TileMap::getTilesRendered() const
    {
        return _tilesRendered;
    }


    Layer* TileMap::getLayer() const
    {
        return _layer;
    }
}
