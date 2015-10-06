#ifndef TILEENGINE_TILESET_HPP
#define TILEENGINE_TILESET_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include "math/geometry/Vector2.hpp"
#include "TileData.hpp"
#include "engine/utils/JsonObject.hpp"

/*
 *         TileSet
 * - Stores basic tile information (texture data, collisions, ...)
 */


namespace engine
{
    class TileSet : public Json::JsonObject
    {
        public:
            TileSet() {};

            using JsonObject::loadFromJson;
            bool loadFromJson(const Json::Value& node);
            void destroy();

            sf::Sprite getSprite(TileID tileid, int offset) const;
            sf::Sprite getSprite(int index) const;

            sf::IntRect getTexRect(TileID tileid, int offset) const;
            sf::IntRect getTexRect(int index) const;

            const TileData& getTile(TileID tileid) const;
            const TileData& getTile(const std::string& name) const;

            bool hasTile(TileID id) const;
            bool hasTile(const std::string& name) const;

            const sf::Texture& getTexSheet() const;
            const geometry::Vector2<int>& getTileSize() const;
            size_t size() const;

        private:
            geometry::Vector2<int> _tilesize;
            std::vector<TileData> _tiles;
            std::map<std::string, const TileData*> _aliases;
            sf::Texture _sheet;
    };
}


/*
 * TileSet file structure:
 *
 * {
 *      "tilew" : <tile width>,
 *      "tileh" : <tile height>,
 *      "tilesheet" : "<path to spritesheet (relative to executable)>",
 *      "tiles" : [
 *          {
 *              "name" : <tilename>
 *              "index" : <tile number on sprite sheet>,
 *              "anilen" : <animation length>,
 *              "anispeed" : <animation speed in frames per second>,
 *              "offset" : <initial frame index>,
 *              "flags" : <bit flags>
 *          },
 *          ...
 *      ]
 * }
 */

#endif
