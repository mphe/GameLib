#ifndef FLOATING_TILE_MAP_HPP
#define FLOATING_TILE_MAP_HPP

#include <memory>
#include <vector>
#include "TileMap.hpp"

namespace engine
{
    class Game;
    class TileSet;
    class FloatingTile;

    typedef std::unique_ptr<FloatingTile> FloatingTilePtr;

    class FloatingTileMap : public TileMap
    {
        public:
            FloatingTileMap(Game* game, const TileSet& tileset);

            using TileMap::loadFromJson;
            bool loadFromJson(const Json::Value& node);
            void destroy();

            void update(float fps);
            void render(sf::RenderTarget& target, const Camera& cam) const;

            TileID getTileID(int x, int y) const;

        private:
            Game* _game;
            const TileSet& _tileset;
            std::vector<FloatingTilePtr> _tiles;
    };
}

/*
 * Config file structure:
 *
 * {
 *     "tiles" : {
 *         <"player" | "cube"> : {
 *             "x" : <x>,
 *             "y" : <y>
 *             "xt" : <tile x>,
 *             "yt" : <tile y>,
 *             ...
 *         },
 *         ...
 *     }
 * }
*/

#endif
