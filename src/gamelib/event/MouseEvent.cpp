#include "gamelib/event/MouseEvent.hpp"

namespace gamelib
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
