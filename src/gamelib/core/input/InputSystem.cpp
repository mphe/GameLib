#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
    constexpr const char* InputSystem::name;

    InputSystem::InputSystem() :
        _window(nullptr)
    { }

    InputSystem::InputSystem(const sf::RenderWindow& win) :
        _window(&win)
    { }

    void InputSystem::setWindow(const sf::RenderWindow& win)
    {
        _window = &win;
    }

    void InputSystem::beginFrame()
    {
        _states.clear();
        _mouse.wheel = 0;
        _mouse.moved = false;
        _consumedMouse = _consumedKeyboard = false;
    }

    void InputSystem::process(const sf::Event& ev)
    {
        if (ev.type == sf::Event::KeyPressed ||
            ev.type == sf::Event::KeyReleased ||
            ev.type == sf::Event::MouseButtonPressed ||
            ev.type == sf::Event::MouseButtonReleased)
        {
            _states.emplace_back();
            auto& state = _states.back();

            switch (ev.type)
            {
                case sf::Event::KeyPressed:
                    state.keyboard = true;
                    state.key = ev.key.code;
                    state.state = Pressed;
                    break;

                case sf::Event::KeyReleased:
                    state.keyboard = true;
                    state.key = ev.key.code;
                    state.state = Released;
                    break;

                case sf::Event::MouseButtonPressed:
                    state.keyboard = false;
                    state.btn = ev.mouseButton.button;
                    state.state = Pressed;
                    break;

                case sf::Event::MouseButtonReleased:
                    state.keyboard = false;
                    state.btn = ev.mouseButton.button;
                    state.state = Released;
                    break;

                default: break; // removes switch warning
            }
        }
        else
        {
            if (ev.type == sf::Event::MouseMoved)
            {
                auto m = sf::Mouse::getPosition(*_window);
                _mouse.desktop = convert(sf::Mouse::getPosition()).asPoint();
                _mouse.win = convert(m).asPoint();
                _mouse.world = convert(_window->mapPixelToCoords(m)).asPoint();
                _mouse.moved = true;
            }
            else if (ev.type == sf::Event::MouseWheelMoved)
                _mouse.wheel = ev.mouseWheel.delta;
        }
    }


    void InputSystem::markConsumed(bool keyboard, bool mouse)
    {
        if (keyboard)
            _consumedKeyboard = true;
        if (mouse)
            _consumedMouse = true;
    }

    bool InputSystem::isKeyboardConsumed() const
    {
        return _consumedKeyboard;
    }

    bool InputSystem::isMouseConsumed() const
    {
        return _consumedMouse;
    }


    bool InputSystem::isKeyPressed(sf::Keyboard::Key key) const
    {
        return isKey(key, Pressed);
    }

    bool InputSystem::isKeyReleased(sf::Keyboard::Key key) const
    {
        return isKey(key, Released);
    }

    bool InputSystem::isKeyDown(sf::Keyboard::Key key) const
    {
        return isKey(key, Down);
    }


    bool InputSystem::isMousePressed(sf::Mouse::Button btn) const
    {
        return isMouse(btn, Pressed);
    }

    bool InputSystem::isMouseReleased(sf::Mouse::Button btn) const
    {
        return isMouse(btn, Released);
    }

    bool InputSystem::isMouseDown(sf::Mouse::Button btn) const
    {
        return isMouse(btn, Down);
    }

    bool InputSystem::isKey(sf::Keyboard::Key key, ButtonState state) const
    {
        return getKeyState(key) == state;
    }

    bool InputSystem::isMouse(sf::Mouse::Button btn, ButtonState state) const
    {
        return getMouseState(btn) == state;
    }


    InputSystem::ButtonState InputSystem::getKeyState(sf::Keyboard::Key key) const
    {
        if (!_check(true, false))
            return None;

        for (auto& i : _states)
            if (i.keyboard && i.key == key)
                return i.state;

        return sf::Keyboard::isKeyPressed(key) ? Down : None;
    }

    InputSystem::ButtonState InputSystem::getMouseState(sf::Mouse::Button btn) const
    {
        if (!_check(false, true))
            return None;

        for (auto& i : _states)
            if (!i.keyboard && i.btn == btn)
                return i.state;

        return sf::Mouse::isButtonPressed(btn) ? Down : None;
    }

    const InputSystem::MouseData& InputSystem::getMouse() const
    {
        return _mouse;
    }

    bool InputSystem::_check(bool keyboard, bool mouse) const
    {
        if ((keyboard && _consumedKeyboard) || (mouse && _consumedMouse))
            return false;
        return _window->hasFocus();
    }
}
