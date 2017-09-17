#ifndef GAMELIB_SPRITE_SET_HPP
#define GAMELIB_SPRITE_SET_HPP

#include <SFML/Graphics.hpp>
#include "gamelib/core/DataSet.hpp"
#include "AnimatedSprite.hpp"
#include "SpriteData.hpp"

namespace gamelib
{
    class SpriteSet : public JsonDataSet<SpriteData>
    {
        public:
            template <bool namespaced = false>
            SpriteSet() :
                JsonDataSet<SpriteData>(_Callback<namespaced>::type) {}

            bool loadFromJson(const Json::Value& node);
            void destroy();

            void setSpriteSheet(const sf::Texture& tex);
            const sf::Texture& getSpriteSheet() const;

            AnimatedSprite getAnimatedSprite(const const_iterator& it) const;
            AnimatedSprite getAnimatedSprite(const DataSetKey& key) const;
            sf::Sprite getSprite(const const_iterator& it) const;
            sf::Sprite getSprite(const DataSetKey& key) const;

        private:
            static bool _loadSprite(const Json::Value& node, SpriteData& spr);

            template <bool namespaced>
            struct _Callback
            {
                static constexpr DataSetCallback<SpriteData> type = _loadSprite;
            };


        private:
            sf::Texture _sheet;
    };

    template <>
    struct SpriteSet::_Callback<true>
    {
        static constexpr DataSetCallback<SpriteData> type = namespaceReader<
            SpriteData, _loadSprite, 's', 'p', 'r', 'i', 't', 'e'>;
    };
}

/*
 * Config file structure (without namespace):
 *
 * {
 *     "sheet": <path to sprite sheet>,
 *     "objects": {
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
 *
 * Config file structure (with namespace):
 *
 * {
 *     "sheet": <path to sprite sheet>,
 *     "objects": {
 *         "<spritename>": {
 *             "sprite": {
 *                 "speed": <animation speed>,
 *                 "length": <animation length>,
 *                 "offset": <animation frame offset>,
 *                 "rect": {
 *                     "x": <x>,
 *                     "y": <y>,
 *                     "w": <width>,
 *                     "h": <height>
 *                 },
 *                 "origin": {
 *                     "x": <x>,
 *                     "y": <y>
 *                 }
 *             }
 *         },
 *         ...
 *     }
 * }
 */

#endif
