#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <queue>
#include <unordered_map>
#include "Event.hpp"
#include "EventHandle.hpp"
#include "gamelib/utils/CallbackHandler.hpp"
#include "gamelib/core/Subsystem.hpp"

namespace gamelib
{
    typedef void (*EventCallback)(void*, EventPtr);

    template <typename T>
    using NiceEventCallback = void (*)(T*, EventPtr);

    void triggerEvent(EventPtr event);
    void queueEvent(EventPtr event);

    // template <typename T, typename U,
    //          typename = decltype(std::static_pointer_cast<U>(std::declval<EventPtr>()))>
    // using NiceEventCallback = void (*)(T*, EventPtr);

    class EventManager : public Subsystem<EventManager>
    {
        friend class EventHandle;

        public:
            ASSIGN_NAMETAG("EventManager");

        public:
            template <class T>
            __attribute__((warn_unused_result))
            auto regCallback(EventID id, NiceEventCallback<T> callback, T* data) -> EventHandle
            {
                return regCallback(id, (EventCallback)callback, data);
            }

            template <class E, class T>
            __attribute__((warn_unused_result))
            auto regCallback(NiceEventCallback<T> callback, T* data) -> EventHandle
            {
                static_assert(has_identifier<E>::value, "Only works for types derived from gamelib::Identifier");
                return regCallback(E::id, callback, data);
            }

            __attribute__((warn_unused_result))
            auto regCallback(EventID id, void (*callback)(void*, EventPtr), void* data) -> EventHandle;

            auto triggerEvent(EventPtr event) -> void;
            auto queueEvent(EventPtr event) -> void;

            template <typename T, typename... Args>
            auto triggerEvent(Args&&... args) -> void
            {
                static_assert(std::is_base_of<BaseEvent, T>::value, "T must be an Event");
                EventPtr ptr(new T(std::forward<Args>(args)...));
                triggerEvent(ptr);
            }

            template <typename T, typename... Args>
            auto queueEvent(Args&&... args) -> void
            {
                static_assert(std::is_base_of<BaseEvent, T>::value, "T must be an Event");
                EventPtr ptr(new T(std::forward<Args>(args)...));
                queueEvent(ptr);
            }

            auto update() -> void;

            auto clearQueue() -> void;

            // Disabled because it would bypass all existing EventHandles,
            // resulting in possible unexpected eventhandler deregistrations
            // when a, now invalid, handle gets unregistered and unregisters
            // a new handler.
            // auto clear() -> void;

            // template <class T>
            // auto unregCallback(EventID id, NiceEventCallback<T> callback, T* data) -> void
            // {
            //     unregCallback(id, (EventCallback)callback, data);
            // }
            //
            // template <class E, class T>
            // auto unregCallback(NiceEventCallback<T> callback, T* data) -> void
            // {
            //     static_assert(has_identifier<E>::value, "Only works for types derived from gamelib::Identifier");
            //     unregCallback(E::id, callback, data);
            // }
            //
            // auto unregCallback(EventID id, void (*callback)(void*, EventPtr), void* data) -> void;

            protected:
                auto _unregCallback(EventID id, void (*callback)(void*, EventPtr), void* data) -> void;

        private:
            std::queue<EventPtr> _evqueue;
            std::unordered_map<EventID, CallbackHandler<void, EventPtr> > _callbacks;
    };


    template <typename E, typename T>
    __attribute__((warn_unused_result))
    EventHandle registerEvent(NiceEventCallback<T> callback, T* data)
    {
        auto evmgr = EventManager::getActive();
        if (evmgr)
            return evmgr->regCallback<E>(callback, data);
        return EventHandle();
    }

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

    // template <typename E, typename T>
    // void unregisterEvent(NiceEventCallback<T> callback, T* data)
    // {
    //     auto evmgr = EventManager::getActive();
    //     if (evmgr)
    //         evmgr->unregCallback<E>(callback, data);
    // }
}

#endif
