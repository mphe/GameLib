#include "gamelib/movement/QController.hpp"
#include "gamelib/ecs/Entity.hpp"
#include "gamelib/movement/QPhysics.hpp"
#include "gamelib/Game.hpp"

namespace gamelib
{
    QController::QController() :
        UpdateComponent(name, 1, UpdateSystem::PreFrame),
        accelerate(10),
        airAccelerate(10),
        maxspeed(100),
        jumpspeed(100),
        qphys(nullptr),
        _canjump(true)
    { }

    void QController::update(float elapsed)
    {
        if (qphys)
        {
            auto game = Game::getActive();

            auto acc = qphys->ground ? accelerate : airAccelerate;

            if (game->isKeyPressed(sf::Keyboard::A))
                qphys->accelerate(math::Vec2f(-1, 0), maxspeed, acc);
            if (game->isKeyPressed(sf::Keyboard::D))
                qphys->accelerate(math::Vec2f(1, 0), maxspeed, acc);

            if (_canjump)
            {
                if (qphys->ground && game->isKeyPressed(sf::Keyboard::W))
                {
                    _canjump = false;
                    qphys->accelerate(math::Vec2f(0, -1), jumpspeed, accelerate);
                }
            }
            else if (!game->isKeyPressed(sf::Keyboard::W))
                _canjump = true;
        }
    }

    bool QController::loadFromJson(const Json::Value& node)
    {
        UpdateComponent::loadFromJson(node);
        maxspeed = node.get("maxspeed", maxspeed).asFloat();
        accelerate = node.get("accelerate", accelerate).asFloat();
        airAccelerate = node.get("airAccelerate", airAccelerate).asFloat();
        jumpspeed = node.get("jumpspeed", jumpspeed).asFloat();

        return true;
    }

    void QController::writeToJson(Json::Value& node)
    {
        UpdateComponent::writeToJson(node);
        node["maxspeed"] = maxspeed;
        node["accelerate"] = accelerate;
        node["airAccelerate"] = airAccelerate;
        node["jumpspeed"] = jumpspeed;
    }

    void QController::_refresh()
    {
        qphys = static_cast<QPhysics*>(
                getEntity()->find(QPhysics::name));
    }
}
