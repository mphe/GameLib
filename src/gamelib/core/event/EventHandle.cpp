#include "gamelib/core/event/EventHandle.hpp"
#include "gamelib/core/event/EventManager.hpp"

namespace gamelib
{
    EventHandle::EventHandle() :
        _evmgr(nullptr),
        _evid(invalidID),
        _callback(nullptr),
        _data(nullptr)
    { }

    EventHandle::EventHandle(EventManager& evmgr, EventID evid, void* callback, void* data) :
        _evmgr(&evmgr),
        _evid(evid),
        _callback(callback),
        _data(data)
    { }

    EventHandle::EventHandle(EventHandle&& other)
    {
        this->operator=(std::move(other));
    }

    EventHandle::~EventHandle()
    {
        unregister();
    }

    EventHandle::operator bool() const
    {
        return _callback && _evmgr;
    }

    auto EventHandle::operator=(EventHandle&& other) -> EventHandle&
    {
        unregister();
        _evmgr = std::move(other._evmgr);
        _evid = std::move(other._evid);
        _callback = std::move(other._callback);
        _data = std::move(other._data);
        other._callback = nullptr;
        return *this;
    }

    auto EventHandle::unregister() -> void
    {
        if (!_callback)
            return;
        if (_evmgr)
            _evmgr->_unregCallback(_evid, (EventCallback)_callback, _data);
        _callback = nullptr;
    }
}
