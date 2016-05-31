#ifndef GAMELIB_RENDERABLE_HPP
#define GAMELIB_RENDERABLE_HPP

namespace sf
{
    class RenderTarget;
}

namespace gamelib
{
    class Renderable
    {
        public:
            virtual ~Renderable() {};
            virtual void render(sf::RenderTarget& target) = 0;
    };
}

#endif
