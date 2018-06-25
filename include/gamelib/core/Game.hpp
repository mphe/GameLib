#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "gamelib/core/res/JsonSerializer.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class GameState;
    typedef std::unique_ptr<GameState> GameStatePtr;

    class Game : public gamelib::JsonSerializer, public Subsystem<Game>
    {
        public:
            constexpr static const char* name = "Game";

        public:
            Game();
            ~Game();

            auto init() -> bool;
            auto run()  -> void;

            // Closes the windows -> stops the game loop.
            // Use this inside the game loop to quit the game.
            auto close() -> void;

            // Cleans up resources (and closes the window).
            // Use this outside the game loop.
            auto destroy() -> void;

            auto loadFromJson(const Json::Value& node) -> bool;

            auto pushState(std::unique_ptr<GameState> state) -> void;
            auto popState()                                  -> void;
            auto pullState() const                           -> GameState&;

            auto getWindow()          -> sf::RenderWindow&;
            auto getFrametime() const -> float;

            auto isFocused() const                         -> bool;
            auto isKeyPressed(sf::Keyboard::Key key) const -> bool;

            auto getMouse() const -> math::Point2f;

        private:
            float _frametime; // TODO: Consider switching to double
            sf::Color _bgcolor;
            sf::RenderWindow _window;
            std::vector<GameStatePtr> _states;
            bool _focused;
            bool _handleclose;
            bool _escclose;
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

