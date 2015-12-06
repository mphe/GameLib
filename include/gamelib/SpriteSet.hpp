#ifndef SPRITE_SET_HPP
#define SPRITE_SET_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "engine/utils/JsonObject.hpp"

namespace engine
{
    typedef int SpriteID;

    struct SpriteData
    {
        SpriteID id;
        int length;
        float speed;
        float offset;
        sf::IntRect rect;
    };

    class SpriteSet : public Json::JsonObject
    {
        public:
            bool loadFromJson(const Json::Value& node);
            void destroy();

            SpriteID add(const std::string& name, const SpriteData& sprdata);
            void setSpriteSheet(const sf::Texture& tex);

            sf::Sprite getSprite(const std::string& name, int offset = 0) const;
            sf::Sprite getSprite(SpriteID id, int offset = 0) const;

            sf::IntRect getTexRect(const std::string& name, int offset = 0) const;
            sf::IntRect getTexRect(SpriteID id, int offset = 0) const;

            const SpriteData& getSpriteData(const std::string& name) const;
            const SpriteData& getSpriteData(SpriteID id) const;

            const sf::Texture& getTexSheet() const;
            size_t size() const;

        private:
            std::vector<SpriteData> _sprites;
            std::map<std::string, SpriteData*> _aliases;
            sf::Texture _sheet;
    };
}

/*
 * Config file structure:
 *
 * {
 *     "sheet": <path to sprite sheet>,
 *     "sprites": {
 *         "<spritename>": {
 *             "speed": <animation speed>,
 *             "length": <animation length>,
 *             "offset": <animation frame offset>,
 *             "rect": {
 *                 "x": <x>,
 *                 "y": <y>,
 *                 "w": <width>,
 *                 "h": <height>
 *             }
 *         },
 *         ...
 *     }
 * }
 */

#endif
