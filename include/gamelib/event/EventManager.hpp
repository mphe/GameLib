#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <queue>
#include <map>
#include "BaseEvent.hpp"
#include "CallbackHandler.hpp"

namespace gamelib
{
    typedef void (*EventCallback)(void*, EventPtr);

    class EventManager
    {
        public:
            void regCallback(EventID id, void (*callback)(void*, EventPtr), void* me = 0);
            void unregCallback(EventID id, void (*callback)(void*, EventPtr), void* me = 0);

            void triggerEvent(EventPtr event) const;
            void queueEvent(EventPtr event);

            void update();

        private:
            std::queue<EventPtr> _evqueue;
            std::map<EventID, CallbackHandler<void, EventPtr> > _callbacks;
    };
}

#endif
