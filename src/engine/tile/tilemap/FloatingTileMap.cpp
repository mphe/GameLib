#include "engine/tile/tilemap/FloatingTileMap.hpp"
#include "engine/tile/FloatingTile.hpp"
#include "engine/tile/TileSet.hpp"
#include "engine/utils/log.hpp"
#include <SFML/Graphics.hpp>

namespace engine
{
    FloatingTileMap::FloatingTileMap(const TileSet& tileset) :
        _tileset(tileset)
    { }

    bool FloatingTileMap::loadFromJson(const Json::Value& node)
    {
        destroy();

        if (node.isMember("tiles"))
        {
            auto& tiles = node["tiles"];
            for (Json::ArrayIndex i = 0; i < tiles.size(); ++i)
            {
                FloatingTilePtr ptr(new FloatingTile(_tileset));
                if (ptr->loadFromJson(tiles[i]))
                {
                    add(std::move(ptr));
                    LOG_DEBUG("FloatingTile ", i, " loaded");
                }
                else
                    LOG_ERROR("Failed to load FloatingTile ", i);
            }
        }
        return true;
    }

    void FloatingTileMap::destroy()
    {
        _tiles.clear();
        LOG_DEBUG_WARN("FloatingTileMap destroyed");
    }


    void FloatingTileMap::update(float fps)
    {
        for (auto& i : _tiles)
        {
            i->update(fps);
        }
    }

    void FloatingTileMap::render(sf::RenderTarget& target, const Camera& cam) const
    {
        for (auto& i : _tiles)
        {
            // if inside cam ...
            i->render(target);
        }
        _tilesRendered = _tiles.size();
    }


    void FloatingTileMap::add(FloatingTilePtr tile)
    {
        _tiles.push_back(std::move(tile));
    }
}
