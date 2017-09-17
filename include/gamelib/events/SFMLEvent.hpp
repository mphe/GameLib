#ifndef GAMELIB_SFML_EVENT_HPP
#define GAMELIB_SFML_EVENT_HPP

#include <SFML/Window/Event.hpp>
#include "gamelib/core/event/Event.hpp"

namespace gamelib
{
    class SFMLEvent : public Event<0x3BFF2E8B, SFMLEvent>
    {
        public:
            SFMLEvent() {}
            SFMLEvent(const sf::Event& ev_) : ev(ev_) {}

        public:
            sf::Event ev;
    };
}

#endif
