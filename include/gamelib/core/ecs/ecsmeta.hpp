#ifndef GAMELIB_ECSMETA_HPP
#define GAMELIB_ECSMETA_HPP

#include <memory>
#include "gamelib/utils/LifetimeTracker.hpp"

namespace gamelib
{
    class Component;
    class Entity;

    typedef std::unique_ptr<Component> ComponentPtr;
    typedef std::unique_ptr<Entity> EntityPtr;
    typedef LifetimeReference<Entity> EntityReference;

    enum RefreshType
    {
        ComponentAdded,     // A component was added to the entity
        ComponentRemoved,   // A component was removed from the entity
        PostLoad,           // All configs were loaded (in entity creation)
    };
}

#endif
