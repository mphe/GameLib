#include "gamelib/utils/Signal.hpp"
#include <utility>

namespace gamelib
{
    SignalHandle::SignalHandle(SignalCallback callback, void* data) :
        _callback(callback),
        _data(data),
        _prev(nullptr),
        _next(nullptr)
    { }

    SignalHandle::SignalHandle(SignalHandle&& other)
    {
        *this = std::move(other);
    }

    SignalHandle::~SignalHandle()
    {
        unregister();
    }

    auto SignalHandle::unregister() -> void
    {
        if (!*this)
            return;

        if (_prev)
            _prev->_next = _next;
        if (_next)
            _next->_prev = _prev;

        // Don't set _prev and _next to nullptr, because those are still
        // needed when unregistering during trigger() loop.
        // _prev = _next = nullptr;

        _callback = nullptr;
        _data = nullptr;
    }

    auto SignalHandle::setData(void* data) -> void
    {
        _data = data;
    }

    auto SignalHandle::getData() const -> void*
    {
        return _data;
    }

    auto SignalHandle::operator=(SignalHandle&& other) -> SignalHandle&
    {
        unregister();

        _callback = other._callback;
        _data = other._data;
        _prev = other._prev;
        _next = other._next;

        // First reset, then update pointers, so that the old handle does not
        // unregister the new one
        other.unregister();

        // fix others' pointers to this handle
        if (*this)
        {
            if (_prev)
                _prev->_next = this;
            if (_next)
                _next->_prev = this;
        }

        return *this;
    }

    auto SignalHandle::operator()(void* arg) const -> void
    {
        if (_callback)
            _callback(arg, _data);
    }

    SignalHandle::operator bool() const
    {
        return _callback && (_prev || _next);
    }



    Signal::Signal()
    { }

    Signal::~Signal()
    {
        clear();
    }

    Signal::Signal(Signal&& other)
    {
        *this = std::move(other);
    }

    auto Signal::operator=(Signal&& other) -> Signal&
    {
        _listeners = std::move(other._listeners);
        other._listeners._next = other._listeners._prev = nullptr;
        return *this;
    }

    auto Signal::trigger(void* arg) const -> void
    {
        // Also check if _listeners._next is null to detect a clear()
        for (SignalHandle* handle = _listeners._next; _listeners._next && handle; handle = handle->_next)
            (*handle)(arg);
    }

    auto Signal::connect(SignalCallback callback, void* data) -> SignalHandle
    {
        SignalHandle handle(callback, data);
        handle._next = _listeners._next;
        handle._prev = &_listeners;
        _listeners._next = &handle;
        return handle;
    }

    auto Signal::size() const -> size_t
    {
        size_t count = 0;
        for (SignalHandle* handle = _listeners._next; handle; handle = handle->_next)
            ++count;
        return count;
    }

    auto Signal::clear() -> void
    {
        while (_listeners._next)
            _listeners._next->unregister();
    }
}
