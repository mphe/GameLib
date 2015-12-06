#ifndef LAYER_HPP
#define LAYER_HPP

#include <string>
#include <vector>
#include <memory>
#include "math/geometry/Vector2.hpp"
#include "gamelib/utils/Factory.hpp"
#include "gamelib/utils/JsonObject.hpp"

namespace sf
{
    class RenderTarget;
}

namespace gamelib
{
    class TileMap;
    class Camera;
    class Scene;

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

    class Layer : public Json::JsonObject
    {
        friend class Scene;

        public:
            typedef Factory<TileMap, std::string> MapFactory;

        public:
            Layer() : _scene(0), _mapfactory(0) {};

            bool loadFromJson(const Json::Value& node);
            void destroy();

            void update(float fps);
            void render(sf::RenderTarget& target, const Camera& cam) const;

            TileMap* addMap(TileMapPtr map, float depth = 1.0);
            TileMap* addMap(const MapData& md);

            void setMapFactory(const MapFactory* factory);

            TileMap* getMap(size_t index) const;
            Scene* getScene() const;
            size_t size() const;
            
        private:
            Scene* _scene;
            const MapFactory* _mapfactory;
            std::vector<MapData> _maps;
    };
}

/*
 * Config file structure:
 *
 * {
 *     <mapid>: {
 *         "paraoffsetx": <parallax x offset>,
 *         "paraoffsety": <parallax y offset>,
 *         "paradepth": <parallax depth>,
 *         "config": <path to this map's config file>,
 *     },
 *     ...
 * }
 */

#endif
