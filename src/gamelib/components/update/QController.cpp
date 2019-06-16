#include "gamelib/components/update/QController.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/input/InputSystem.hpp"

namespace gamelib
{
    QController::QController() :
        UpdateComponent(1, PreFrame),
        accelerate(10),
        airAccelerate(10),
        maxspeed(100),
        jumpspeed(100),
        jumpDecelerate(500),
        phys(nullptr),
        slopejumps(true),
        handleInput(true),
        _input(0),
        _oldinput(0),
        _canjump(true),
        _jumping(false)
    {
        _props.registerProperty("accelerate", accelerate);
        _props.registerProperty("airAccelerate", airAccelerate);
        _props.registerProperty("maxspeed", maxspeed);
        _props.registerProperty("jumpspeed", jumpspeed);
        _props.registerProperty("jumpDecelerate", jumpDecelerate);
        _props.registerProperty("slopejumps", slopejumps);
        _props.registerProperty("handleInput", handleInput);
    }

    void QController::update(float elapsed)
    {
        if (handleInput)
        {
            auto input = InputSystem::getActive();
            unsigned int buttons = 0;

            if (input->isKeyDown(sf::Keyboard::A))
                buttons |= input_left;

            if (input->isKeyDown(sf::Keyboard::D))
                buttons |= input_right;

            if (input->isKeyDown(sf::Keyboard::W))
                buttons |= input_up;

            setInput(buttons);
        }

        if (phys)
        {
            bool onground = phys->getState() == QPhysics::Ground;
            auto acc = onground ? accelerate : airAccelerate;
            math::Vec2f wishdir(1, 0);

            if (onground)
            {
                // wishdir = phys->getGround().normal.right();
                phys->airFriction = false;
                _jumping = false;
            }

            if (_input & input_left)
                phys->accelerate(-wishdir, maxspeed, acc);
            else if (!onground && _released(input_left))
                phys->airFriction = true;

            if (_input & input_right)
                phys->accelerate(wishdir, maxspeed, acc);
            else if (!onground && _released(input_right))
                phys->airFriction = true;

            if (!(_input & input_up))
                _canjump = true;

            if (_canjump && onground && _input & input_up)
            {
                _canjump = false;
                _jumping = true;

                if (!slopejumps || phys->vel.y > 0)
                    phys->vel.y = -jumpspeed;
                else
                    phys->vel.y -= jumpspeed;

                if (phys->basevel.y > 0)
                    phys->basevel.y = 0;
            }

            if (_jumping && !(_input & input_up))
                phys->vel.y += jumpDecelerate * elapsed;
        }

        _oldinput = _input;
        _input = 0;
    }

    auto QController::setInput(unsigned int input) -> void
    {
        _input = input;
    }

    auto QController::getInput() const -> unsigned int
    {
        return _input;
    }

    auto QController::_released(unsigned int button) const -> bool
    {
        return _oldinput & button && !(_input & button);
    }

    void QController::_refresh(RefreshType type, Component* comp)
    {
        phys = getEntity()->findByName<QPhysics>();
    }
}
