#include "gamelib/ecs/EntityManager.hpp"
#include "gamelib/ecs/EntityFactory.hpp"
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
        return const_cast<EntityManager*>(this)->find(name);
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

    bool EntityManager::loadFromJson(const Json::Value& node)
    {
        auto factory = getSubsystem<EntityFactory>();
        assert(factory != nullptr && "Requires EntityFactory");

        if (node.get("clear", true).asBool())
            clear();

        auto& ents = node["entities"];
        if (!ents.isNull())
            for (auto& i : ents)
                factory->createFromJson(i);
        return true;
    }

    void EntityManager::writeToJson(Json::Value& node)
    {
        node["clear"] = true;
        auto& entities = node["entities"];
        for (auto& i : _entities)
            i.writeToJson(entities.append(Json::Value()));
    }
}
