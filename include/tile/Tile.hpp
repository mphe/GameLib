#ifndef TILEENGINE_TILE_HPP
#define TILEENGINE_TILE_HPP

#include <SFML/Graphics.hpp>
#include "TileData.hpp"
#include "utils/JsonObject.hpp"

namespace engine
{
    class Tile : public Json::JsonObject
    {
        public:
            Tile() {}
            Tile(const TileData& data) : _tiledata(data) {}
            virtual ~Tile() {};

            virtual void update(float fps);

            const TileData& getTileData() const;

        protected:
            TileData _tiledata;
    };
}

#endif
