#include "gamelib/event/KeyEvent.hpp"

namespace gamelib
{
    KeyEvent::KeyEvent(sf::Event::EventType type_, const sf::Event::KeyEvent& ev_) :
        type(type_),
        ev(ev_)
    {}

    EventID KeyEvent::getID() const
    {
        return id;
    }
}
