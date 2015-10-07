#ifndef STATIC_TILEMAP_HPP
#define STATIC_TILEMAP_HPP

#include <vector>
#include <string>
#include "TileMap.hpp"
#include "math/geometry/Vector2.hpp"

namespace sf
{
    class RenderTarget;
}

namespace engine
{
    class TileSet;

    constexpr int noRepeat = 0;
    constexpr int infiniteRepeat = -1;

    class StaticTileMap : public TileMap
    {
        public:
            StaticTileMap(const TileSet& tileset);
            virtual ~StaticTileMap() {}

            void update(float fps);
            void render(sf::RenderTarget& surface, const Camera& cam) const;

            bool loadFromJson(const Json::Value& node);
            void destroy();

            void setRepeatSize(int w, int h);

            const TileSet& getTileSet() const;
            TileID getTileID(int x, int y) const;
            bool hasFlags(int x, int y, int flags) const;

            // Return NULL if there's no tile
            Tile* getTile(int x, int y);
            const Tile* getTile(int x, int y) const;

        private:
            TileID _get(int tilex, int tiley) const;

        private:
            const TileSet& _tileset;
            geometry::Vector2<int> _size;
            geometry::Vector2<int> _repeat;
            std::vector<TileID> _map;
            std::vector<Tile> _tiles;
    };
}

/*
 * Config file structure:
 *
 * {
 *      "width" : <width>,
 *      "height" : <height>,
 *      "virtwidth" : <virtual width>,
 *      "virtheight" : <virtual height>,
 *      "tiles" : [<tileid/alias>, ...],
 *      "alias" : {
 *              "<alias>" : <tileid>,
 *              ...
 *      }
 * }
*/
#endif
