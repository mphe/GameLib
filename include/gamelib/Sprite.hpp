#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP

#include <SFML/Graphics.hpp>

namespace engine
{
    struct AnimationData
    {
        AnimationData();
        int startx;
        int starty;
        int length;
        float speed;
        float offset;
    };

    class AnimatedSprite
    {
        public:
            AnimatedSprite() {}
            AnimatedSprite(const sf::Texture& tex) : sfmlsprite(tex) {}
            AnimatedSprite(const sf::Texture& tex, const sf::IntRect& rect) : sfmlsprite(tex, rect) {}
            virtual ~AnimatedSprite() {};

            void update(float fps);

        public:
            AnimationData anidata;
            sf::Sprite sfmlsprite;
    };
}

#endif
