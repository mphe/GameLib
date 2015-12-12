#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <queue>
#include <map>
#include "BaseEvent.hpp"
#include "CallbackHandler.hpp"

namespace gamelib
{
    typedef CallbackHandler<void(EventPtr)>::CallbackFunction EventCallback;

    class EventManager
    {
        public:
            void regCallback(EventID id, const EventCallback& callback);
            void unregCallback(EventID id, const EventCallback& callback);

            void triggerEvent(EventPtr event) const;
            void queueEvent(EventPtr event);

            void update();

        private:
            std::queue<EventPtr> _evqueue;
            std::map<EventID, CallbackHandler<void(EventPtr)> > _callbacks;
    };
}

#endif
