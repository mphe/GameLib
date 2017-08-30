#include "gamelib/ecs/EntityManager.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Entity& EntityManager::add(const std::string& name)
    {
        auto h = _entities.acquire();
        Entity& ent = _entities[h];
        ent._name = name;
        ent._handle = h;
        ent._entmgr = this;
        return ent;
    }

    Entity& EntityManager::add(Entity&& entity)
    {
        Entity& ent = add();
        ent = std::move(entity);
        return ent;
    }

    const Entity* EntityManager::get(Handle handle) const
    {
        return const_cast<EntityManager*>(this)->get(handle);
    }

    Entity* EntityManager::get(Handle handle)
    {
        if (_entities.isValid(handle))
            return &_entities[handle];

        // LOG_DEBUG_WARN("Trying to use invalid entity handle");
        return nullptr;
    }

    const Entity* EntityManager::find(const std::string& name) const
    {
        for (auto& i : _entities)
            if (i.getName() == name)
                return &i;
        return nullptr;
    }

    Entity* EntityManager::find(const std::string& name)
    {
        return const_cast<EntityManager*>(this)->find(name);
    }

    void EntityManager::destroy(Handle handle)
    {
        if (_entities.isValid(handle))
        {
            // No need to reset _entmgr field, because the object is deleted
            // anyway
            _entities[handle]._quit();
            _entities.destroy(handle);
        }
    }

    void EntityManager::clear()
    {
        for (auto& i : _entities)
            i._quit();
        _entities.clear();
    }
}
