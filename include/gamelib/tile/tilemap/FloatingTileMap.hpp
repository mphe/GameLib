#ifndef FLOATING_TILE_MAP_HPP
#define FLOATING_TILE_MAP_HPP

#include <memory>
#include <vector>
#include "TileMap.hpp"
#include "gamelib/tile/FloatingTile.hpp"

namespace gamelib
{
    class Game;
    class TileSet;

    typedef std::unique_ptr<FloatingTile> FloatingTilePtr;

    class FloatingTileMap : public TileMap
    {
        public:
            FloatingTileMap(const TileSet& tileset);
            virtual ~FloatingTileMap() {}

            virtual bool loadFromJson(const Json::Value& node);
            virtual void destroy();

            virtual void update(float fps);
            virtual void render(sf::RenderTarget& target, const Camera& cam) const;

            virtual void add(FloatingTilePtr tile);
            virtual void add(const std::string& tile, float x, float y);
            virtual void add(TileID tile, float x, float y);

        protected:
            const TileSet& _tileset;
            std::vector<FloatingTilePtr> _tiles;
    };
}

/*
 * Config file structure:
 *
 * {
 *     "tiles" : [
 *         {
 *             "tileid" : <tileid>
 *             "x" : <x>,
 *             "y" : <y>
 *             "xt" : <tile x>,
 *             "yt" : <tile y>,
 *         },
 *         ...
 *     ]
 * }
*/

#endif
