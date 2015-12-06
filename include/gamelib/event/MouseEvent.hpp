#ifndef GAMELIB_MOUSE_EVENT_HPP
#define GAMELIB_MOUSE_EVENT_HPP

#include <SFML/Window/Event.hpp>
#include "Event.hpp"

namespace gamelib
{
    class MouseEvent : public Event
    {
        public:
            static constexpr EventID id = 0xF348A1D6;

            union SFMLMouseEvent
            {
                sf::Event::MouseMoveEvent move;
                sf::Event::MouseButtonEvent button;
                sf::Event::MouseWheelScrollEvent scroll;
                sf::Event::MouseWheelEvent wheel;

                SFMLMouseEvent(const sf::Event::MouseMoveEvent& ev) : move(ev) {}
                SFMLMouseEvent(const sf::Event::MouseWheelScrollEvent& ev) : scroll(ev) {}
                SFMLMouseEvent(const sf::Event::MouseWheelEvent& ev) : wheel(ev) {}
                SFMLMouseEvent(const sf::Event::MouseButtonEvent& ev) : button(ev) {}
            };

        public:
            MouseEvent(sf::Event::EventType type_, const SFMLMouseEvent& ev_);
            EventID getID() const;

        public:
            sf::Event::EventType type;
            SFMLMouseEvent ev;
    };
}

#endif
