#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "gamelib/res/JsonSerializer.hpp"
#include "event/EventManager.hpp"
#include "gamelib/Subsystem.hpp"

namespace gamelib
{
    class GameState;
    typedef std::unique_ptr<GameState> GameStatePtr;

    class Game : public gamelib::JsonSerializer, public Subsystem<Game>
    {
        public:
            Game();
            ~Game();

            bool init();
            void run();

            // Closes the windows -> stops the game loop.
            // Use this inside the game loop to quit the game.
            void close();

            // Cleans up resources (and closes the window).
            // Use this outside the game loop.
            void destroy();

            bool loadFromJson(const Json::Value& node);

            void pushState(std::unique_ptr<GameState> state);
            void popState();
            GameState& pullState() const;

            sf::RenderWindow& getWindow();
            EventManager& getEventManager();

            bool isActive() const;
            bool isKeyPressed(sf::Keyboard::Key key) const;

        private:
            sf::Color _bgcolor;
            sf::RenderWindow _window;
            std::vector<GameStatePtr> _states;
            bool _active;
            bool _handleclose;
            bool _escclose;
            EventManager _evmgr;
    };
}

/*
 * Config file structure:
 * {
 *     "title": <title>,
 *     "width": <window width>,
 *     "height": <window height>,
 *     "maxfps": <fps limit, 0 for no limit>,
 *     "vsync": <true|false>,
 *     "handleclose": <close when close button is pressed: true|false>,
 *     "escclose": <close when escape is pressed: true|false>,
 *     "repeatkeys": <true|false>,
 *     "bg": {
 *         "r": <red>,
 *         "g": <green>,
 *         "b": <blue>
 *     }
 * }
 */

#endif

