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

        get()->owner = this;   // TODO: Maybe move this to constructor
        sys->add(get());
        return true;
    }

    void CollisionComponent::_quit()
    {
        System::getActive()->getCollisionSystem()->remove(get());
    }
}
