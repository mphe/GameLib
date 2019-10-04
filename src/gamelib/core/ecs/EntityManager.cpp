#include "gamelib/core/ecs/EntityManager.hpp"

namespace gamelib
{
    EntityReference findEntity(const std::string& name)
    {
        return EntityManager::getActive()->find(name);
    }


    EntityManager::EntityManager() :
        _root(root_entity_name)
    { }

    EntityReference EntityManager::add(const std::string& name)
    {
        return getRoot()->addChild(EntityPtr(new Entity(name)));
    }

    EntityReference EntityManager::find(const std::string& name) const
    {
        return foreach([&](Entity* ent) {
            return ent->getName() == name;
        });
    }

    void EntityManager::clear()
    {
        _root.clear();
    }

    auto EntityManager::getRoot() const -> EntityReference
    {
        return &_root;
    }
}
