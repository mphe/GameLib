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
            ASSIGN_NAMETAG("InputSystem");

            enum ButtonState
            {
                None,
                Pressed,
                Released,
                Down,
            };

            struct MouseData
            {
                math::Point2i desktop;
                math::Point2i win;
                math::Point2f world;
                int wheel;
                bool moved;
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

            auto isPressed(sf::Keyboard::Key key) const        -> bool;
            auto isReleased(sf::Keyboard::Key key) const       -> bool;
            auto isDown(sf::Keyboard::Key key) const           -> bool;
            auto isDownDirect(sf::Keyboard::Key key) const     -> bool;
            auto isLastDownDirect(sf::Keyboard::Key key) const -> bool;

            auto isPressed(sf::Mouse::Button btn) const        -> bool;
            auto isReleased(sf::Mouse::Button btn) const       -> bool;
            auto isDown(sf::Mouse::Button btn) const           -> bool;
            auto isDownDirect(sf::Mouse::Button btn) const     -> bool;
            auto isLastDownDirect(sf::Mouse::Button btn) const -> bool;

            auto getMouse() const -> const MouseData&;

        public:
            MouseData _mouse;
            const sf::RenderWindow* _window;

            unsigned int _currentBuffer;
            bool _mbuttons[2][sf::Mouse::ButtonCount];
            bool _keys[2][sf::Keyboard::Key::KeyCount];

            bool _consumedKeyboard;
            bool _consumedMouse;
    };
}

#endif
