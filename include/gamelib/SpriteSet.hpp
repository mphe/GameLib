#ifndef SPRITE_SET_HPP
#define SPRITE_SET_HPP

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "gamelib/utils/JsonObject.hpp"
#include "Sprite.hpp"

namespace gamelib
{
    typedef std::string SpriteID;

    class SpriteSet : public Json::JsonObject
    {
        private:
            // SpriteSet uses SpriteData to store sprite settings, instead of using
            // sf::Sprite or gamelib::Sprite, to be more memory efficient.
            // Maybe this will change later.
            struct SpriteData
            {
                int length;
                float speed;
                float offset;
                sf::IntRect rect;
            };

        public:
            auto loadFromJson(const Json::Value& node)  -> bool;
            auto destroy()                              -> void;

            auto add(const SpriteID& key, const SpriteData& spr)    -> void;
            auto setSpriteSheet(const sf::Texture& tex)             -> void;

            auto getTexRect(const SpriteID& key, int offset = 0)    const -> sf::IntRect;
            auto getSprite(const SpriteID& key)                     const -> const Sprite;
            auto getSFMLSprite(const SpriteID& key, int offset = 0) const -> sf::Sprite;
            auto getSpriteSheet()                                   const -> const sf::Texture&;
            auto size()                                             const -> size_t;

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
 *             }
 *         },
 *         ...
 *     }
 * }
 */

#endif
