#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>
#include <memory>
#include "math/geometry/Vector2.hpp"

namespace sf
{
    class RenderTarget;
}

namespace engine
{
    class TileMap;
    class Camera;

    typedef std::unique_ptr<TileMap> TileMapPtr;

    struct MapData
    {
        float depth; // multiplier for parallax
        geometry::Vector2<int> offset; // parallax offset
        mutable TileMapPtr map; //mutable -> making const copy-constructor possible

        MapData();
        MapData(const MapData& md);
        MapData& operator=(const MapData& md);
    };

    class Layer
    {
        public:
            void destroy();

            void update(float fps);
            void render(sf::RenderTarget& target, const Camera& cam) const;

            TileMap* addMap(TileMapPtr map, float depth = 1.0);
            TileMap* addMap(const MapData& md);

            TileMap* getMap(size_t index) const;
            size_t size() const;
            
        private:
            std::vector<MapData> _maps;
    };
}

#endif
