#include "gamelib/events/ResourceReloadEvent.hpp"

namespace gamelib
{
    ResourceReloadEvent::ResourceReloadEvent(const std::string& fname_, BaseResourceHandle ptr) :
        fname(fname_),
        res(ptr)
    { }
}
