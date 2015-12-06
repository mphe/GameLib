#ifndef GAMELIB_KEY_EVENT_HPP
#define GAMELIB_KEY_EVENT_HPP

#include <SFML/Window/Event.hpp>
#include "Event.hpp"

namespace gamelib
{
    class KeyEvent : public Event
    {
        public:
            static constexpr EventID id = 0xE2D55872;

        public:
            KeyEvent(sf::Event::EventType type_, const sf::Event::KeyEvent& ev_);
            EventID getID() const;

        public:
            sf::Event::EventType type;
            sf::Event::KeyEvent ev;
    };
}

#endif
