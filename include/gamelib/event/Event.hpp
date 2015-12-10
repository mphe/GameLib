#ifndef GAMELIB_EVENT_HPP
#define GAMELIB_EVENT_HPP

#include "BaseEvent.hpp"

// Simple wrapper around BaseEvent.
// It takes an optional event ID as template parameter and provides a getID() method, which returns the given ID.
// If no ID is passed, invalidEvent will be used.
// Additionally a static (constexpr) variable "id" is set to store the ID.

namespace gamelib
{
    template <int ID = invalidEvent>
    class Event : public BaseEvent
    {
        public:
            static constexpr EventID id = ID;

        public:
            virtual ~Event() {}

            EventID getID() const
            {
                return id;
            }
    };
}

#endif
