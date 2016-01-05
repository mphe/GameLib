#include "gamelib/component/Entity.hpp"
#include "gamelib/utils/log.hpp"
#include <algorithm>

namespace gamelib
{
    void Entity::add(ComponentPtr ptr)
    {
        ptr->setEntity(this);
        _components.push_back(std::move(ptr));
        LOG_DEBUG("Component ", _components.back()->getID(), " added");
    }

    void Entity::remove(ComponentID id)
    {
        auto it = std::find_if(_components.begin(), _components.end(),
                [&](const ComponentPtr& ptr) { return ptr->getID() == id; });
        if (it != _components.end())
        {
            (*it)->setEntity(nullptr);
            _components.erase(it);
            LOG_DEBUG_WARN("Component ", (*it)->getID(), " removed");
        }
    }

    Component* Entity::get(ComponentID id) const
    {
        auto it = std::find_if(_components.begin(), _components.end(),
                [&](const ComponentPtr& ptr) { return ptr->getID() == id; });
        return it != _components.end() ? it->get() : nullptr;
    }

    void Entity::clear()
    {
        _components.clear();
        LOG_DEBUG_WARN("Entity cleared");
    }

    size_t Entity::size() const
    {
        return _components.size();
    }
}
