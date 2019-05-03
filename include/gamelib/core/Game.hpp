#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "gamelib/json/JsonSerializer.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "math/geometry/Point2.hpp"
#include "gamelib/properties/PropertyContainer.hpp"

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

            auto loadFromJson(const Json::Value& node) -> bool final override;
            auto writeToJson(Json::Value& node) const  -> void final override;

            auto pushState(GameStatePtr state) -> void;
            auto popState()                    -> void;
            auto pullState() const             -> GameState*;

            auto getWindow()              -> sf::RenderWindow&;
            auto getFrametime() const     -> float;
            auto getRealFrametime() const -> float;
            auto getRenderTime() const    -> float;
            auto getUpdateTime() const    -> float;
            auto getProperties() const    -> const PropertyContainer&;

            auto resize(const sf::Vector2u& size) -> void;

        public:
            sf::Color bgcolor;
            bool closebutton;
            bool escclose;
            bool unfocusPause;

        private:
            float _frametime; // TODO: Consider switching to double
            float _rendertime;
            float _updatetime;
            math::Vec2i _size;
            int _maxfps;
            std::string _title;
            bool _repeatkeys;
            bool _vsync;
            sf::RenderWindow _window;
            std::vector<GameStatePtr> _states;
            PropertyContainer _props;
    };
}

#endif

