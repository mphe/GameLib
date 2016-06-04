#ifndef STATIC_TILEMAP_HPP
#define STATIC_TILEMAP_HPP

#include <vector>
#include <string>
#include "math/geometry/AABB.hpp"
#include "gamelib/utils/JsonObject.hpp"
#include "gamelib/sprite/SpriteSet.hpp"
#include "gamelib/Updatable.hpp"

namespace sf
{
    class RenderTarget;
}

namespace gamelib
{
    class SpriteSet;

    constexpr int noRepeat = 0;
    constexpr int infiniteRepeat = -1;

    typedef int TileID;
    constexpr int InvalidTile = -1;

    class StaticTileMap : public Json::JsonObject, public Updatable
    {
        public:
            StaticTileMap(const SpriteSet& spriteset);
            virtual ~StaticTileMap() {}

            void update(float fps);
            // Make a copy from rect because it will be modified
            void render(sf::RenderTarget& surface, geometry::AABB<int> rect);

            bool loadFromJson(const Json::Value& node);
            void destroy();

            void setVirtualSize(int w, int h);

        private:
            typedef std::unordered_map<SpriteID, TileID> TranslationMap;

        private:
            TileID _get(int tilex, int tiley) const;
            TranslationMap _copySprites(const Json::Value& node);

            // Applies the repeat-rules to the given coordinate and returns
            // the proper array index.
            // <val> = the coordinate's current value
            // <index> = the coordinate's index (x = 0, y = 1)
            int _adaptCoords(int val, int index) const;

        private:
            const SpriteSet& _spriteset;
            int _size[2]; // x = 0, y = 1
            int _virtsize[2]; // same here
            int _tsize[2]; // same here
            size_t _tilesRendered;
            std::vector<TileID> _map;
            std::vector<AnimationData> _tiles;
    };
}

/*
 * Config file structure:
 *
 * {
 *      "tilew": <tile width>,
 *      "tileh": <tile height>,
 *      "width" : <width>,
 *      "height" : <height>,
 *      "virtwidth" : <virtual width>,
 *      "virtheight" : <virtual height>,
 *      "sprites": [ "spritename1", "spritename2", ... ],
 *      "tiles" : [<tileid/alias>, ...],
 *      "alias" : {
 *              "<alias>" : <tileid>,
 *              ...
 *      }
 * }
*/
#endif
