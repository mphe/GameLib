#include "engine/event/EventManager.hpp"

namespace engine
{
    void EventManager::regCallback(EventID id, void (*callback)(void*, EventPtr), void* me)
    {
        _callbacks[id].regCallback(callback, me);
    }

    void EventManager::unregCallback(EventID id, void (*callback)(void*, EventPtr), void* me)
    {
        _callbacks.at(id).unregCallback(callback, me);
    }

    void EventManager::triggerEvent(EventPtr event) const 
    {
        auto it = _callbacks.find(event->getID());
        if (it != _callbacks.end())
        {
            it->second.call(event);
        }
    }

    void EventManager::queueEvent(EventPtr event) 
    {
        _evqueue.push(event); 
    }

    void EventManager::update()
    {
        while(!_evqueue.empty())
        {
            triggerEvent(_evqueue.front());
            _evqueue.pop();
        }
    }
}
