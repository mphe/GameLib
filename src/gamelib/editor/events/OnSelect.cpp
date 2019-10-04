#include "gamelib/editor/events/OnSelect.hpp"

namespace gamelib
{
    OnSelectEvent::OnSelectEvent(EntityReference old_, EntityReference ent) :
        old(old_),
        entity(ent)
    { }
}
