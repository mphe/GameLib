#include "event/KeyEvent.hpp"

namespace engine
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
