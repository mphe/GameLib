#include "gamelib/ecs/Component.hpp"
#include "gamelib/ecs/EntityManager.hpp"
#include "gamelib/System.hpp"

namespace gamelib
{
    Component::Component()
    { }

    Entity* Component::getEntity() const
    {
        return System::getActive()->getEntityManager()->get(_ent);
    }

    Component::Handle Component::getEntityHandle() const
    {
        return _ent;
    }

    bool Component::_init()
    {
        return true;
    }

    void Component::_quit()
    { }

    void Component::_refresh()
    { }
}
