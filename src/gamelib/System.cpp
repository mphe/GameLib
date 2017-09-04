#include "gamelib/System.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Scene* System::getScene()
    {
        LOG_WARN("Trying to access non-existing Scene object");
        return nullptr;
    }

    CollisionSystem* System::getCollisionSystem()
    {
        LOG_WARN("Trying to access non-existing CollisionSystem object");
        return nullptr;
    }

    UpdateSystem* System::getUpdateSystem()
    {
        LOG_WARN("Trying to access non-existing UpdateSystem object");
        return nullptr;
    }

    EventManager* System::getEventManager()
    {
        LOG_WARN("Trying to access non-existing EventManager object");
        return nullptr;
    }

    ResourceManager* System::getResourceManager()
    {
        LOG_WARN("Trying to access non-existing ResourceManager object");
        return nullptr;
    }

    EntityManager* System::getEntityManager()
    {
        LOG_WARN("Trying to access non-existing EntityManager object");
        return nullptr;
    }

    EntityFactory* System::getEntityFactory()
    {
        LOG_WARN("Trying to access non-existing EntityFactory object");
        return nullptr;
    }

    Game* System::getGame()
    {
        LOG_WARN("Trying to access non-existing Game object");
        return nullptr;
    }

    void System::setActive()
    {
        _active = this;
    }

    System* System::getActive()
    {
        return _active;
    }


    System* System::_active = nullptr;
}
