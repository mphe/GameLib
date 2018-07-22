#include "gamelib/components/update/QController.hpp"
#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/input/InputSystem.hpp"

namespace gamelib
{
    QController::QController() :
        UpdateComponent(name, 1, UpdateSystem::PreFrame),
        accelerate(10),
        airAccelerate(10),
        maxspeed(100),
        jumpspeed(100),
        jumpDecelerate(500),
        phys(nullptr),
        _canjump(true)
    {
        _props.registerProperty("accelerate", accelerate);
        _props.registerProperty("airAccelerate", airAccelerate);
        _props.registerProperty("maxspeed", maxspeed);
        _props.registerProperty("jumpspeed", jumpspeed);
        _props.registerProperty("jumpDecelerate", jumpDecelerate);
    }

    void QController::update(float elapsed)
    {
        if (phys)
        {
            auto input = InputSystem::getActive();
            bool onground = phys->getState() == QPhysics::Ground;
            auto acc = onground ? accelerate : airAccelerate;
            math::Vec2f wishdir(1, 0);

            if (onground)
            {
                wishdir = phys->getGround().normal.ortho();
                phys->airFriction = false;
            }

            if (input->isKeyDown(sf::Keyboard::A))
                phys->accelerate(-wishdir, maxspeed, acc);
            else if (!onground && input->isKeyReleased(sf::Keyboard::A))
                phys->airFriction = true;
                // phys->vel.x = std::min(phys->vel.x + maxspeed, 0.f);

            if (input->isKeyDown(sf::Keyboard::D))
                phys->accelerate(wishdir, maxspeed, acc);
            else if (!onground && input->isKeyReleased(sf::Keyboard::D))
                phys->airFriction = true;
                // phys->vel.x = std::max(phys->vel.x - maxspeed, 0.f);

            if (!input->isKeyDown(sf::Keyboard::W))
                _canjump = true;

            if (_canjump && onground && input->isKeyDown(sf::Keyboard::W))
            {
                _canjump = false;
                phys->vel.y = -jumpspeed;
                if (phys->basevel.y > 0)
                    phys->basevel.y = 0;
            }

            if (!onground && !input->isKeyDown(sf::Keyboard::W))
                phys->vel.y += jumpDecelerate * elapsed;
        }
    }

    void QController::_refresh()
    {
        phys = getEntity()->findByName<QPhysics>();
    }
}
