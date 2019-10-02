#include "gamelib/utils/LifetimeTracker.hpp"

namespace gamelib
{
    SlotMapShort<void*> LifetimeTrackerManager::_data;

    LifetimeTrackerManager::LifetimeTrackerManager()
    { }

    auto LifetimeTrackerManager::add(void* ptr) -> LifetimeHandle
    {
        auto h = _data.acquire();
        _data[h] = ptr;
        return h;
    }

    auto LifetimeTrackerManager::del(LifetimeHandle handle) -> void
    {
        _data.destroy(handle);
    }

    auto LifetimeTrackerManager::get(LifetimeHandle handle) -> void*
    {
        if (_data.isValid(handle))
            return _data[handle];
        return nullptr;
    }

    auto LifetimeTrackerManager::update(LifetimeHandle handle, void* newptr) -> void
    {
        if (_data.isValid(handle))
            _data[handle] = newptr;
    }
}
