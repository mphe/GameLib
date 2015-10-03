#include "tile/tilemap/FloatingTileMap.hpp"
#include "tile/FloatingTile.hpp"
#include "tile/TileSet.hpp"
#include "utils/log.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace engine
{
    FloatingTileMap::FloatingTileMap(Game* game, const TileSet& tileset) :
        _game(game),
        _tileset(tileset)
    { }

    bool FloatingTileMap::loadFromJson(const Json::Value& node)
    {
        destroy();

        if (node.isMember("tiles"))
        {
            ///////////////////////////////////////
            // TODO: outsource this to a factory //
            ///////////////////////////////////////

            auto& tiles = node["tiles"];
            for (auto i = tiles.begin(); i != tiles.end(); ++i)
            {
                // if (i->name == "player")
                //     _tiles.push_back(FloatingTilePtr(new gravity::Player(_game, _tileset, i->value)));
                // else
                //     LOG_WARN("Tile \"", i->name.GetString(), "\" is not implemented -> Skipping");
            }
        }
        return true;
    }

    void FloatingTileMap::destroy()
    {
        _tiles.clear();
        LOG_WARN("FloatingTileMap destroyed");
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


    TileID FloatingTileMap::getTileID(int x, int y) const
    {
        throw "Not implemented";
    }
}
