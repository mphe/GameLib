#ifndef EDITOR_EVENT_ONSELECT_HPP
#define EDITOR_EVENT_ONSELECT_HPP

#include "gamelib/core/event/Event.hpp"
#include "gamelib/core/ecs/Entity.hpp"

namespace gamelib
{
    class OnSelectEvent : public Event<0x69e801b0, OnSelectEvent>
    {
        public:
            OnSelectEvent() {};
            OnSelectEvent(Entity::Handle old, Entity::Handle ent);

            auto getOld() const -> Entity*;
            auto getNew() const -> Entity*;

        public:
            Entity::Handle old;
            Entity::Handle entity;
    };
}

#endif
