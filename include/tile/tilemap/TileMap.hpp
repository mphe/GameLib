#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include "../Tile.hpp"
#include "utils/JsonObject.hpp"

namespace sf
{
    class RenderTarget;
}

namespace engine
{
    class Camera;
    class ObjectSet;
    class TileSet;

    class TileMap : public Json::JsonObject
    {
        public:
            TileMap() : _tilesRendered(0) {}
            virtual ~TileMap() {}

            virtual void destroy() = 0;

            virtual void update(float fps) = 0;
            virtual void render(sf::RenderTarget& target, const Camera& cam) const = 0;

            virtual TileID getTileID(int x, int y) const = 0;
            // virtual const Tile* getTile(int x, int y) const = 0;
            // virtual ObjectSet& getObjectSet() const;

        public:
            inline int getTilesRendered() const { return _tilesRendered; }

        protected:
            mutable int _tilesRendered;
    };
}

#endif
