#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/utils/log.hpp"
#include <cassert>

namespace gamelib
{
    constexpr const char* EntityManager::name;

    Entity::Handle EntityManager::add(const std::string& name)
    {
        auto h = _entities.acquire();
        Entity& ent = _entities[h];
        ent._name = name;
        ent._handle = h;
        ent._entmgr = this;
        return h;
    }

    // Entity& EntityManager::add(Entity&& entity)
    // {
    //     Entity& ent = add();
    //     ent = std::move(entity);
    //     ent._init();
    //     return ent;
    // }

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
        for (auto& i : _entities)
            if (i.getName() == name)
                return &i;
        return nullptr;
    }

    void EntityManager::destroy(Handle handle)
    {
        if (_entities.isValid(handle))
        {
            _entities[handle]._quit();
            _entities[handle]._entmgr = nullptr;
            LOG_DEBUG("Entity destroyed: ", _entities[handle].getName());
            _entities.destroy(handle);
        }
    }

    void EntityManager::clear()
    {
        for (auto& i : _entities)
            i._quit();
        _entities.clear();
    }


    EntityManager::Container::const_iterator EntityManager::begin() const
    {
        return _entities.begin();
    }

    EntityManager::Container::iterator EntityManager::begin()
    {
        return _entities.begin();
    }

    EntityManager::Container::const_iterator EntityManager::end() const
    {
        return _entities.end();
    }

    EntityManager::Container::iterator EntityManager::end()
    {
        return _entities.end();
    }
}
