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

    void triggerEvent(EventPtr event);
    void queueEvent(EventPtr event);

    // template <typename T, typename... Args>
    // void triggerEvent(Args&&... args);
    //
    // template <typename T, typename... Args>
    // void queueEvent(Args&&... args);


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
                static_assert(has_identifier<E>::value, "Only works for types derived from gamelib::Identifier");
                regCallback(E::id, callback, data);
            }

            template <class E, class T>
            void unregCallback(NiceEventCallback<T> callback, T* data)
            {
                static_assert(has_identifier<E>::value, "Only works for types derived from gamelib::Identifier");
                unregCallback(E::id, callback, data);
            }

            void regCallback(EventID id, void (*callback)(void*, EventPtr), void* data);
            void unregCallback(EventID id, void (*callback)(void*, EventPtr), void* data);

            void triggerEvent(EventPtr event);
            void queueEvent(EventPtr event);

            template <typename T, typename... Args>
            void triggerEvent(Args&&... args)
            {
                static_assert(std::is_base_of<BaseEvent, T>::value, "T must be an Event");
                EventPtr ptr(new T(std::forward<Args>(args)...));
                triggerEvent(ptr);
            }

            template <typename T, typename... Args>
            void queueEvent(Args&&... args)
            {
                static_assert(std::is_base_of<BaseEvent, T>::value, "T must be an Event");
                EventPtr ptr(new T(std::forward<Args>(args)...));
                queueEvent(ptr);
            }

            void update();

            void clear();

        private:
            std::queue<EventPtr> _evqueue;
            std::unordered_map<EventID, CallbackHandler<void, EventPtr> > _callbacks;
    };


    // Implementation
    template <typename T, typename... Args>
    void triggerEvent(Args&&... args)
    {
        auto evmgr = EventManager::getActive();
        if (evmgr)
            evmgr->triggerEvent<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    void queueEvent(Args&&... args)
    {
        auto evmgr = EventManager::getActive();
        if (evmgr)
            evmgr->queueEvent<T>(std::forward<Args>(args)...);
    }
}

#endif
