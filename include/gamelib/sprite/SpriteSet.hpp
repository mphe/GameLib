#ifndef GAMELIB_SPRITE_SET_HPP
#define GAMELIB_SPRITE_SET_HPP

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
            typedef std::unordered_map<SpriteID, SpriteData> SpriteMap;
            typedef SpriteMap::iterator iterator;
            typedef SpriteMap::const_iterator const_iterator;
            typedef SpriteMap::difference_type difference_type;
            typedef SpriteMap::size_type size_type;
            typedef SpriteMap::value_type value_type;
            typedef SpriteMap::key_type key_type;
            typedef SpriteMap::pointer pointer;
            typedef SpriteMap::reference reference;

        public:
            auto loadFromJson(const Json::Value& node)  -> bool;
            auto destroy()                              -> void;

            auto add(const SpriteID& key, const SpriteData& spr)    -> void;
            auto add(SpriteID&& key, const SpriteData& spr)    -> void;

            auto setSpriteSheet(const sf::Texture& tex)             -> void;

            auto getAnimatedSprite(const SpriteID& key)   const -> AnimatedSprite;
            auto getSprite(const SpriteID& key)           const -> sf::Sprite;
            auto getSpriteData(const SpriteID& key)       const -> const SpriteData&;
            auto hasSprite(const SpriteID& key)           const -> bool;

            auto getAnimatedSprite(const const_iterator& it)   const -> AnimatedSprite;
            auto getSprite(const const_iterator& it)           const -> sf::Sprite;

            auto getSpriteSheet()                         const -> const sf::Texture&;
            auto size()                                   const -> size_t;

            auto begin()       -> iterator;
            auto begin() const -> const_iterator;

            auto end()       -> iterator;
            auto end() const -> const_iterator;

            auto find (const SpriteID& key)         -> iterator;
            auto find (const SpriteID& key) const   -> const_iterator;

        private:
            SpriteMap _sprites;
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
