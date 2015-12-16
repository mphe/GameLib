#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <queue>
#include <map>
#include "Event.hpp"
#include "CallbackHandler.hpp"

namespace gamelib
{
    typedef void (*EventCallback)(void*, EventPtr);

    class EventManager
    {
        public:
            void regCallback(EventID id, void (*callback)(void*, EventPtr), void* me);
            void unregCallback(EventID id, void (*callback)(void*, EventPtr), void* me);

            void triggerEvent(EventPtr event);
            void queueEvent(EventPtr event);

            void update();

        private:
            std::queue<EventPtr> _evqueue;
            std::map<EventID, CallbackHandler<void, EventPtr> > _callbacks;
    };
}

#endif
