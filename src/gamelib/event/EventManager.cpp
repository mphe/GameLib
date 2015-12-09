#include "gamelib/event/EventManager.hpp"

namespace gamelib
{
    void EventManager::regCallback(EventID id, void (*callback)(void*, EventPtr), void* me)
    {
        _callbacks[id].regCallback(callback, me);
    }

    void EventManager::unregCallback(EventID id, void (*callback)(void*, EventPtr), void* me)
    {
        auto it = _callbacks.find(id);
        if (it == _callbacks.end())
            return;
        it->second.unregCallback(callback, me);

        if (!it->second.size()) // remove the callback handler if it is empty
            _callbacks.erase(it);
    }

    void EventManager::triggerEvent(EventPtr event) const
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
}
