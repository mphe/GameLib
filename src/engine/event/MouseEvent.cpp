#include "engine/event/MouseEvent.hpp"

namespace engine
{
    MouseEvent::MouseEvent(sf::Event::EventType type_, const SFMLMouseEvent& ev_) :
        type(type_),
        ev(ev_)
    {}

    EventID MouseEvent::getID() const
    {
        return id;
    }
}
