#ifndef SPRITE_SET_HPP
#define SPRITE_SET_HPP

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "gamelib/utils/JsonObject.hpp"
#include "AnimatedSprite.hpp"
#include "SpriteData.hpp"

namespace gamelib
{
    typedef std::string SpriteID;

    class SpriteSet : public Json::JsonObject
    {
        public:
            auto loadFromJson(const Json::Value& node)  -> bool;
            auto destroy()                              -> void;

            auto add(const SpriteID& key, const SpriteData& spr)    -> void;
            auto setSpriteSheet(const sf::Texture& tex)             -> void;

            auto getAnimatedSprite(const SpriteID& key)   const -> AnimatedSprite;
            auto getSprite(const SpriteID& key)           const -> sf::Sprite;
            auto getSpriteData(const SpriteID& key)       const -> const SpriteData&;

            auto getSpriteSheet()                         const -> const sf::Texture&;
            auto size()                                   const -> size_t;

        private:
            std::unordered_map<SpriteID, SpriteData> _sprites;
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
 *             },
 *             "origin": {
 *                 "x": <x>,
 *                 "y": <y>
 *             }
 *         },
 *         ...
 *     }
 * }
 */

#endif
