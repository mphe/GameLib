#include "gamelib/event/EventManager.hpp"

namespace gamelib
{
    void EventManager::regCallback(EventID id, const EventCallback& callback)
    {
        _callbacks[id].regCallback(callback);
    }

    void EventManager::unregCallback(EventID id, const EventCallback& callback)
    {
        auto it = _callbacks.find(id);
        if (it == _callbacks.end())
            return;

        it->second.unregCallback(callback);

        // remove the callback handler if it is empty
        if (!it->second.size())
            _callbacks.erase(it);
    }

    void EventManager::triggerEvent(EventPtr event) const
    {
        auto it = _callbacks.find(event->getID());
        if (it == _callbacks.end())
            return;

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
