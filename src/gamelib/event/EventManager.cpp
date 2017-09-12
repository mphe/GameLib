#include "gamelib/event/EventManager.hpp"

namespace gamelib
{
    constexpr const char* EventManager::name;

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
}
