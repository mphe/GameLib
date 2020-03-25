#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    EntityReference createEntity(const std::string& name)
    {
        return getSubsystem<EntityFactory>()->create(name);
    }

    EntityReference createEntity(const std::string& name, float x, float y)
    {
        auto ent = createEntity(name);
        if (ent)
            ent->getTransform().setPosition(x, y);
        return ent;
    }

    EntityFactory::EntityFactory()
    { }


    EntityReference EntityFactory::createWithDelta(const Json::Value& node)
    {
        EntityPtr ent(new Entity());

        if (createWithDelta(node, ent.get()))
            return getSubsystem<EntityManager>()->getRoot()->addChild(std::move(ent));
        return nullptr;
    }

    bool EntityFactory::createWithDelta(const Json::Value& node, Entity* ent)
    {
        if (!node.isMember("name"))
        {
            LOG_ERROR("Invalid entity config format: No name specified");
            return false;
        }

        auto name = node["name"].asString();

        if (create(name, ent) && extendFromJson(node, *ent))
            return true;
        return false;
    }


    EntityReference EntityFactory::create(const std::string& name)
    {
        auto found = _findTemplate(name);
        if (found)
            return createFromJson(found->getConfig());
        return nullptr;
    }

    bool EntityFactory::create(const std::string& name, Entity* ent)
    {
        auto found = _findTemplate(name);
        if (found)
            return createFromJson(found->getConfig(), ent);
        return false;
    }

    EntityReference EntityFactory::createFromJson(const Json::Value& node)
    {
        auto ent = getSubsystem<EntityManager>()->add();

        if (!createFromJson(node, ent.get()))
        {
            ent->destroy();
            ent = nullptr;
        }
        return ent;
    }

    bool EntityFactory::createFromJson(const Json::Value& node, Entity* ent)
    {
        assert(ent && "ent is null");
        return loadFromJson(node, *ent);
    }


    ComponentPtr EntityFactory::createComponent(const std::string& name)
    {
        return _compfactory.create(name);
    }

    ComponentPtr EntityFactory::createComponentFromJson(const std::string& name, const Json::Value& node)
    {
        auto ptr = createComponent(name);
        if (ptr && ptr->loadFromJson(node))
            return ptr;
        LOG_ERROR("Failed to create component ", name);
        return nullptr;
    }

    auto EntityFactory::add(EntityResource::Handle entcfg) -> void
    {
        const auto name = entcfg->getName();
        if (name.empty())
        {
            LOG_ERROR("Entity name must not be empty");
            return;
        }

        _entdata[name] = entcfg;
        LOG_DEBUG("Registered entity ", name);
    }

    void EntityFactory::addComponent(const std::string& name, ComponentFactory::CreatorFunction callback)
    {
        if (name.empty())
        {
            LOG_ERROR("Entity name must not be empty");
            return;
        }
        _compfactory.add(name, callback);
        LOG_DEBUG("Registered component ", name);
    }

    void EntityFactory::removeEntity(const std::string& name)
    {
        auto it = _entdata.find(name);
        if (it != _entdata.end())
            _entdata.erase(it);
    }

    void EntityFactory::removeComponent(const std::string& name)
    {
        _compfactory.remove(name);
    }

    void EntityFactory::clear()
    {
        _compfactory.clear();
        _entdata.clear();
    }

    size_t EntityFactory::size() const
    {
        return _entdata.size();
    }

    EntityResource::Handle EntityFactory::findEntity(const std::string& name) const
    {
        auto it = _entdata.find(name);
        if (it != _entdata.end())
            return it->second;
        return nullptr;
    }

    auto EntityFactory::_findTemplate(const std::string& name) -> EntityResource::Handle
    {
        auto found = findEntity(name);
        if (!found)
        {
            auto resmgr = ResourceManager::getActive();
            if (resmgr)
            {
                // If found, the entity autoregisters at the factory.
                // Won't work if EntityFactory::getActive() returns another factory than this.
                found = resmgr->getOnce(name + ".ent");
                if (!found)
                    found = resmgr->getOnce(name + ".entity");
            }
        }
        return found;
    }
}
