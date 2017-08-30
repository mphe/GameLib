#include "gamelib/ecs/CollisionComponent.hpp"
#include "gamelib/System.hpp"
#include "gamelib/collision/CollisionSystem.hpp"

namespace gamelib
{
    bool CollisionComponent::_init()
    {
        auto sys = System::getActive()->getCollisionSystem();
        if (!sys)
            return false;

        sys->add(this);
        return true;
    }

    void CollisionComponent::_quit()
    {
        System::getActive()->getCollisionSystem()->remove(this);
    }
}
