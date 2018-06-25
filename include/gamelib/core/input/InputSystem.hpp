#ifndef GAMELIB_INPUT_SYSTEM_HPP
#define GAMELIB_INPUT_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include "math/geometry/Vector.hpp"
#include "gamelib/core/Subsystem.hpp"

namespace gamelib
{
    class InputSystem : public Subsystem<InputSystem>
    {
        public:
            static constexpr const char* name = "InputSystem";

            enum ButtonState
            {
                Pressed,
                Released,
                Down,
                None
            };

            struct MouseData
            {
                math::Point2i desktop;
                math::Point2i win;
                math::Point2f world;
                int wheel;
            };

        public:
            InputSystem();
            InputSystem(const sf::RenderWindow& win);

            auto setWindow(const sf::RenderWindow& win) -> void;

            auto beginFrame()                 -> void;
            auto process(const sf::Event& ev) -> void;

            auto markConsumed(bool keyboard, bool mouse) -> void;
            auto isKeyboardConsumed() const -> bool;
            auto isMouseConsumed() const    -> bool;

            auto isKeyPressed(sf::Keyboard::Key key) const  -> bool;
            auto isKeyReleased(sf::Keyboard::Key key) const -> bool;
            auto isKeyDown(sf::Keyboard::Key key) const     -> bool;

            auto isMousePressed(sf::Mouse::Button btn) const  -> bool;
            auto isMouseReleased(sf::Mouse::Button btn) const -> bool;
            auto isMouseDown(sf::Mouse::Button btn) const     -> bool;

            auto isKey(sf::Keyboard::Key key, ButtonState state) const   -> bool;
            auto isMouse(sf::Mouse::Button btn, ButtonState state) const -> bool;

            auto getKeyState(sf::Keyboard::Key key) const   -> ButtonState;
            auto getMouseState(sf::Mouse::Button btn) const -> ButtonState;

            auto getMouse() const -> const MouseData&;

        private:
            auto _check(bool keyboard, bool mouse) const -> bool;

        private:
            struct StateStruct
            {
                bool keyboard;
                ButtonState state;
                union {
                    sf::Mouse::Button btn;
                    sf::Keyboard::Key key;
                };
            };

        private:
            MouseData _mouse;
            const sf::RenderWindow* _window;
            bool _consumedKeyboard;
            bool _consumedMouse;
            std::vector<StateStruct> _states;
    };
}

#endif
