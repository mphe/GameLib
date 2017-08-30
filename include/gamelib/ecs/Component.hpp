#ifndef GAMELIB_COMPONENT_HPP
#define GAMELIB_COMPONENT_HPP

#include "gamelib/Identifiable.hpp"
#include "gamelib/utils/SlotMap.hpp"

// TODO: Get the System from Entity

namespace gamelib
{
    class Entity;

    class Component : public Identifiable
    {
        friend class Entity;

        typedef SlotKeyShort Handle;

        public:
            Component();
            virtual ~Component() {};

            auto getEntity() const       -> Entity*;
            auto getEntityHandle() const -> Handle;

        protected:
            // Those are called by Entity
            virtual auto _init()    -> bool;
            virtual auto _quit()    -> void;
            virtual auto _refresh() -> void;

        private:
            Handle _ent;   // Set by Entity

            // Only used when the entity was created outside an EntityManager
            Entity* _entptr; // Set by Entity
    };
}

#endif
