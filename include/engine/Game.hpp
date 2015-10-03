#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "engine/utils/JsonObject.hpp"
// #include "event/EventManager.hpp"

namespace engine
{
    class GameState;
    typedef std::unique_ptr<GameState> GameStatePtr;

    class Game : public Json::JsonObject
    {
        public:
            Game();
            ~Game();

            bool init();
            void run();

            // Closes the windows -> stops the game loop
            void close();

            // Cleans up resources (and closes the window)
            void destroy();

            using JsonObject::loadFromJson;
            bool loadFromJson(const Json::Value& node);

            void pushState(std::unique_ptr<GameState> state);
            void popState();
            GameState& pullState() const;

            bool isActive() const;
            bool isKeyPressed(sf::Keyboard::Key key) const;

            // void regEventCallback(EventID id, void (*callback)(void*, EventPtr), void* me = 0);
            // void unregEventCallback(EventID id, void (*callback)(void*, EventPtr), void* me = 0);

        private:
            sf::RenderWindow _window;
            std::vector<GameStatePtr> _states;
            bool _active;
            // EventManager _evmgr;
    };
}

/*
 * Config file structure:
 * {
 *     "title": <title>,
 *     "width": <window width>,
 *     "height": <window height>,
 *     "maxfps": <fps limit, 0 for no limit>,
 *     "vsync": <true|false>
 * }
 */

#endif

