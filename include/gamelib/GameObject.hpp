#ifndef GAMELIB_GAME_OBJECT_HPP
#define GAMELIB_GAME_OBJECT_HPP

namespace sf
{
    class RenderTarget;
}

namespace gamelib
{
    class GameObject
    {
        public:
            virtual ~GameObject() {};
            virtual void update(float fps) = 0;
            virtual void render(sf::RenderTarget& target) const = 0;
    };
}

#endif
