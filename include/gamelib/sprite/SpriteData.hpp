#ifndef GAMELIB_SPRITE_DATA_HPP
#define GAMELIB_SPRITE_DATA_HPP

#include <SFML/Graphics/Rect.hpp>

namespace gamelib
{
    class AnimationData
    {
        public:
            AnimationData();
            void update(float fps);
            sf::IntRect getRect(int texw, int texh) const;

        public:
            int length;
            float speed;
            float offset;
            sf::IntRect rect;
    };

    // More memory efficient than storing sf::Sprite or gamelib::AnimatedSprite
    // if a drawable sprite object isn't needed.
    struct SpriteData
    {
        AnimationData anidata;
        sf::Vector2f origin;
    };
}

#endif
