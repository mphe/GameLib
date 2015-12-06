#ifndef GAMELIB_SPRITE_HPP
#define GAMELIB_SPRITE_HPP

#include <SFML/Graphics.hpp>

namespace gamelib
{
    class Sprite
    {
        public:
            Sprite() {}
            Sprite(const sf::Texture& tex) : sfsprite(tex) {}
            Sprite(const sf::Texture& tex, const sf::IntRect& rect) : sfsprite(tex, rect) {}
            virtual ~Sprite() {};

            void update(float fps);

        public:
            int length;
            float speed;
            float offset;
            int startx;
            int starty;
            sf::Sprite sfsprite;
    };
}

#endif
