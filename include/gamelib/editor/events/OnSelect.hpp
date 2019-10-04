#ifndef EDITOR_EVENT_ONSELECT_HPP
#define EDITOR_EVENT_ONSELECT_HPP

#include "gamelib/core/event/Event.hpp"
#include "gamelib/core/ecs/ecsmeta.hpp"

namespace gamelib
{
    class OnSelectEvent : public Event<0x69e801b0, OnSelectEvent>
    {
        public:
            OnSelectEvent() {};
            OnSelectEvent(EntityReference old, EntityReference ent);

        public:
            EntityReference old;
            EntityReference entity;
    };
}

#endif
