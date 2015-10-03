#ifndef EVENT_HPP
#define EVENT_HPP

#include <memory>

namespace engine
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
