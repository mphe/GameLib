#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <SFML/Graphics/RenderTarget.hpp>

namespace gamelib
{
    class Game;

    class GameState
    {
        public:
            virtual ~GameState() {};

            virtual bool init(Game* game) = 0;
            virtual void quit() = 0;

            virtual void update(float fps) = 0;
            virtual void render(sf::RenderTarget& target) const = 0;
    };
}

#endif

