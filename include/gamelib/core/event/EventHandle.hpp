#ifndef GAMELIB_EVENTHANDLE_HPP
#define GAMELIB_EVENTHANDLE_HPP

#include "Event.hpp"

namespace gamelib
{
    class EventManager;

    class EventHandle
    {
        public:
            EventHandle();
            EventHandle(EventManager& evmgr, EventID evid, void* callback, void* data);
            EventHandle(const EventHandle&) = delete;
            EventHandle(EventHandle&&);
            ~EventHandle();

            auto operator=(const EventHandle&) -> EventHandle& = delete;
            auto operator=(EventHandle&&) -> EventHandle&;

            explicit operator bool() const;

            auto unregister() -> void;

        private:
            EventManager* _evmgr;
            EventID _evid;
            void* _callback;
            void* _data;
    };
}

#endif
