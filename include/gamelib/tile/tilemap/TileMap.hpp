#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include "gamelib/tile/Tile.hpp"
#include "gamelib/utils/JsonObject.hpp"

namespace sf
{
    class RenderTarget;
}

namespace gamelib
{
    class Camera;
    class Layer;

    class TileMap : public Json::JsonObject
    {
        friend class Layer;

        public:
            TileMap();
            virtual ~TileMap() {}

            virtual void destroy() = 0;

            virtual void update(float fps) = 0;
            virtual void render(sf::RenderTarget& target, const Camera& cam) const = 0;

            int getTilesRendered() const;
            Layer* getLayer() const;

        protected:
            mutable int _tilesRendered; // mutable to be able to change it inside a const render function
            Layer* _layer;
    };
}

#endif
