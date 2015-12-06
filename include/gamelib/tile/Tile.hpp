#ifndef TILEENGINE_TILE_HPP
#define TILEENGINE_TILE_HPP

#include <SFML/Graphics.hpp>
#include "TileData.hpp"
#include "gamelib/utils/JsonObject.hpp"

namespace gamelib
{
    class TileSet;

    class Tile : public Json::JsonObject
    {
        public:
            Tile(const TileSet& tileset, TileID id = InvalidTile);
            // Tile(const TileData& data) : _tiledata(data) {}
            virtual ~Tile() {};

            virtual void update(float fps);

            const TileData& getTileData() const;

        protected:
            void _change(TileID id);
            sf::IntRect _getTexRect() const;

        protected:
            const TileSet& _tileset;
            TileData _tiledata;
    };
}

#endif
