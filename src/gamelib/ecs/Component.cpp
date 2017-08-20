#include "gamelib/ecs/Component.hpp"

namespace gamelib
{
    Component::Component() :
        _ent(nullptr)
    { }

    Entity* Component::getEntity() const
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
