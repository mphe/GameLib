#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <queue>
#include <unordered_map>
#include "Event.hpp"
#include "CallbackHandler.hpp"
#include "gamelib/core/Subsystem.hpp"

namespace gamelib
{
    typedef void (*EventCallback)(void*, EventPtr);

    template <typename T>
    using NiceEventCallback = void (*)(T*, EventPtr);

    class EventManager : public Subsystem<EventManager>
    {
        public:
            ASSIGN_NAMETAG("EventManager");

        public:
            template <class T>
            void regCallback(EventID id, NiceEventCallback<T> callback, T* data)
            {
                regCallback(id, (EventCallback)callback, data);
            }

            template <class T>
            void unregCallback(EventID id, NiceEventCallback<T> callback, T* data)
            {
                unregCallback(id, (EventCallback)callback, data);
            }

            template <class E, class T>
            void regCallback(NiceEventCallback<T> callback, T* data)
            {
                static_assert(isIdentifiable<E>::value, "Only works for types derived from gamelib::Identifier");
                regCallback(E::id, callback, data);
            }

            template <class E, class T>
            void unregCallback(NiceEventCallback<T> callback, T* data)
            {
                static_assert(isIdentifiable<E>::value, "Only works for types derived from gamelib::Identifier");
                unregCallback(E::id, callback, data);
            }

            void regCallback(EventID id, void (*callback)(void*, EventPtr), void* data);
            void unregCallback(EventID id, void (*callback)(void*, EventPtr), void* data);

            void triggerEvent(EventPtr event);
            void queueEvent(EventPtr event);

            void update();

            void clear();

        private:
            std::queue<EventPtr> _evqueue;
            std::unordered_map<EventID, CallbackHandler<void, EventPtr> > _callbacks;
    };
}

#endif
