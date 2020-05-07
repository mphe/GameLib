#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/utils/conversions.hpp"
#include "imgui.h"

namespace gamelib
{
    InputSystem::InputSystem() :
        _window(nullptr),
        _currentBuffer(0),
        _mbuttons{},
        _keys{}
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
        _mouse.wheel = 0;
        _mouse.moved = false;
        _mouse.world = convert(_window->mapPixelToCoords(sf::Mouse::getPosition(*_window))).asPoint();
        _consumedMouse = _consumedKeyboard = false;

        // Flip and update buffer
        _currentBuffer ^= 1;
        for (size_t i = 0; i < sf::Keyboard::KeyCount; ++i)
            _keys[_currentBuffer][i] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i));
        for (size_t i = 0; i < sf::Mouse::ButtonCount; ++i)
            _mbuttons[_currentBuffer][i] = sf::Mouse::isButtonPressed(sf::Mouse::Button(i));
    }

    void InputSystem::process(const sf::Event& ev)
    {
        if (ev.type == sf::Event::MouseMoved)
        {
            auto m = sf::Mouse::getPosition(*_window);
            _mouse.desktop = convert(sf::Mouse::getPosition()).asPoint();
            _mouse.win = convert(m).asPoint();
            _mouse.moved = true;
        }
        else if (ev.type == sf::Event::MouseWheelMoved)
            _mouse.wheel = ev.mouseWheel.delta;
    }


    void InputSystem::markConsumed(bool keyboard, bool mouse)
    {
        _consumedKeyboard |= keyboard;
        _consumedMouse |= mouse;
    }

    bool InputSystem::isKeyboardConsumed() const
    {
        return _consumedKeyboard || ImGui::GetIO().WantCaptureKeyboard
                || (_window && !_window->hasFocus());
    }

    bool InputSystem::isMouseConsumed() const
    {
        return _consumedMouse || ImGui::GetIO().WantCaptureMouse
            || (_window && !_window->hasFocus());
    }


    bool InputSystem::isPressed(sf::Keyboard::Key key) const
    {
        return !isLastDownDirect(key) && isDown(key);
    }

    bool InputSystem::isReleased(sf::Keyboard::Key key) const
    {
        return isLastDownDirect(key) && !isDown(key);
    }

    bool InputSystem::isDown(sf::Keyboard::Key key) const
    {
        return isDownDirect(key) && !isKeyboardConsumed();
    }

    bool InputSystem::isDownDirect(sf::Keyboard::Key key) const
    {
        return _keys[_currentBuffer][key];
    }

    bool InputSystem::isLastDownDirect(sf::Keyboard::Key key) const
    {
        return _keys[_currentBuffer ^ 1][key];
    }


    bool InputSystem::isPressed(sf::Mouse::Button btn) const
    {
        return !isLastDownDirect(btn) && isDown(btn);
    }

    bool InputSystem::isReleased(sf::Mouse::Button btn) const
    {
        return isLastDownDirect(btn) && !isDown(btn);
    }

    bool InputSystem::isDown(sf::Mouse::Button btn) const
    {
        return isDownDirect(btn) && !isMouseConsumed();
    }

    bool InputSystem::isDownDirect(sf::Mouse::Button btn) const
    {
        return _mbuttons[_currentBuffer][btn];
    }

    bool InputSystem::isLastDownDirect(sf::Mouse::Button btn) const
    {
        return _mbuttons[_currentBuffer ^ 1][btn];
    }


    const InputSystem::MouseData& InputSystem::getMouse() const
    {
        return _mouse;
    }
}
