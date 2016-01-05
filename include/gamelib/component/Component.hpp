#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include <memory>
#include "gamelib/Identifiable.hpp"

/*
 * Base class for components.
 * Defines an empty virtual function "setEntity" that can be overloaded by
 * derived classes. It's not pure virtual because not all components care
 * about their entity.
 */

namespace gamelib
{
    class Entity;

    class Component : public Identifiable
    {
        public:
            virtual ~Component() {}

            // Gets called after the component is added to an entity.
            virtual void setEntity(Entity* entity) {}
    };

    typedef ID ComponentID;
    typedef std::unique_ptr<Component> ComponentPtr;
}

#endif
