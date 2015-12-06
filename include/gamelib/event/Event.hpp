#ifndef GAMELIB_EVENT_HPP
#define GAMELIB_EVENT_HPP

#include <memory>

namespace gamelib
{
    typedef unsigned int EventID;

    constexpr EventID invalidEvent = -1;

    class Event
    {
        public:
            virtual ~Event() {}
            virtual EventID getID() const = 0;
    };

    typedef std::shared_ptr<Event> EventPtr;
}

#endif
