#ifndef GAMELIB_RESOURCE_RELOAD_EVENT_HPP
#define GAMELIB_RESOURCE_RELOAD_EVENT_HPP

#include "gamelib/core/event/Event.hpp"
#include "gamelib/core/res/Resource.hpp"

namespace gamelib
{
    class ResourceReloadEvent : public Event<0x05eb2119, ResourceReloadEvent>
    {
        public:
            ResourceReloadEvent(const std::string& fname, BaseResourceHandle ptr);

            // The new resource's filename (relative to search path)
            std::string fname;

            // The new resource
            BaseResourceHandle res;
    };
}

#endif
