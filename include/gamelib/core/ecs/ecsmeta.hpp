#ifndef GAMELIB_ECSMETA_HPP
#define GAMELIB_ECSMETA_HPP

#include <memory>
#include "gamelib/utils/LifetimeTracker.hpp"

namespace gamelib
{
    class Component;
    class Entity;

    typedef std::unique_ptr<Component>   ComponentPtr;
    typedef std::unique_ptr<Entity>      EntityPtr;
    typedef LifetimeReference<Entity>    EntityReference;
    typedef EntityReference              EntRef;
    typedef LifetimeReference<Component> BaseCompRef;

    template <typename T = Component> //, typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type> (doesn't work -> incomplete type)
    using ComponentReference = LifetimeReference<T>;

    template <typename T = Component>
    using CompRef = ComponentReference<T>;

    enum RefreshType
    {
        ComponentAdded,     // A component was added to the entity
        ComponentRemoved,   // A component was removed from the entity
        PostLoad,           // All configs were loaded (in entity creation)
    };
}

#endif
