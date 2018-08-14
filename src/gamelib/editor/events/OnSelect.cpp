#include "gamelib/editor/events/OnSelect.hpp"

namespace gamelib
{
    OnSelectEvent::OnSelectEvent(Entity::Handle old_, Entity::Handle ent) :
        old(old_),
        entity(ent)
    { }

    Entity* OnSelectEvent::getOld() const
    {
        return getEntity(old);
    }

    Entity* OnSelectEvent::getNew() const
    {
        return getEntity(entity);
    }
}
