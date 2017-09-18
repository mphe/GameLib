#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "gamelib/utils/bitflags.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

namespace gamelib
{
    class Game;

    constexpr unsigned int gamestate_paused      = 1;       // don't update
    constexpr unsigned int gamestate_freeze      = 1 << 1;  // don't update lower states
    constexpr unsigned int gamestate_forceupdate = 1 << 2;  // ignore gamestate_freeze

    class GameState
    {
        public:
            GameState() : flags(0) {};
            GameState(unsigned int flags_) : flags(flags_) {};
            virtual ~GameState() {};

            virtual bool init(Game* game) = 0;
            virtual void quit() = 0;

            virtual void update(float elapsed) = 0;
            virtual void render(sf::RenderTarget& target) = 0;

        public:
            unsigned int flags;
    };
}

#endif

