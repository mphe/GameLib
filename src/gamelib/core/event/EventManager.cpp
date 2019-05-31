#include "gamelib/core/event/EventManager.hpp"

namespace gamelib
{
    void EventManager::triggerEvent(EventPtr event)
    {
        auto it = _callbacks.find(event->getID());
        if (it != _callbacks.end())
            it->second.call(event);
    }

    void EventManager::queueEvent(EventPtr event)
    {
        _evqueue.push(event);
    }

    void EventManager::update()
    {
        while (!_evqueue.empty())
        {
            triggerEvent(_evqueue.front());
            _evqueue.pop();
        }
    }

    void EventManager::regCallback(EventID id, void (*callback)(void*, EventPtr), void* data)
    {
        _callbacks[id].regCallback(callback, data);
    }

    void EventManager::unregCallback(EventID id, void (*callback)(void*, EventPtr), void* data)
    {
        auto it = _callbacks.find(id);
        if (it == _callbacks.end())
            return;

        it->second.unregCallback((EventCallback)callback, data);

        if (!it->second.size()) // remove the callback handler if it is empty
            _callbacks.erase(it);
    }

    void EventManager::clear()
    {
        _callbacks.clear();
        while (!_evqueue.empty())
            _evqueue.pop();
    }
}
