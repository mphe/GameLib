#ifndef GAMELIB_BASE_EVENT_HPP
#define GAMELIB_BASE_EVENT_HPP

#include <memory>

// Base class for events.
// It only defines a virtual method to get the event ID.
// If you plan to write your own Event, consider inheriting from Event instead of BaseEvent (see Event.hpp).

namespace gamelib
{
    typedef int EventID;

    constexpr EventID invalidEvent = -1;

    class BaseEvent
    {
        public:
            virtual ~BaseEvent() {}
            virtual EventID getID() const = 0;
    };

    typedef std::shared_ptr<BaseEvent> EventPtr;
}

#endif
