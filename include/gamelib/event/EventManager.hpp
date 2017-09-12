#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <queue>
#include <map>
#include "Event.hpp"
#include "CallbackHandler.hpp"
#include "gamelib/Subsystem.hpp"

namespace gamelib
{
    typedef void (*EventCallback)(void*, EventPtr);

    class EventManager : public Subsystem<EventManager>
    {
        public:
            constexpr static const char* name = "EventManager";

        public:
            template <class T>
            void regCallback(EventID id, void (*callback)(T*, EventPtr), T* me)
            {
                _callbacks[id].regCallback((EventCallback)callback, me);
            }

            template <class T>
            void unregCallback(EventID id, void (*callback)(T*, EventPtr), T* me)
            {
                auto it = _callbacks.find(id);
                if (it == _callbacks.end())
                    return;
                it->second.unregCallback((EventCallback)callback, me);

                if (!it->second.size()) // remove the callback handler if it is empty
                    _callbacks.erase(it);
            }


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
