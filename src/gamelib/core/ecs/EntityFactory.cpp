#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    EntityHandle createEntity(const std::string& name)
    {
        return getSubsystem<EntityFactory>()->create(name);
    }

    EntityHandle createEntity(const std::string& name, float x, float y)
    {
        auto h = createEntity(name);
        if (h)
            getEntity(h)->getTransform().setPosition(x, y);
        return h;
    }

    EntityFactory::EntityFactory()
    { }


    EntityHandle EntityFactory::createWithDelta(const std::string& name, const Json::Value& node)
    {
        auto entmgr = getSubsystem<EntityManager>();
        auto handle = entmgr->add();

        if (!createWithDelta(name, node, getEntity(handle)))
        {
            entmgr->destroy(handle);
            handle.reset();
        }

        return handle;
    }

    bool EntityFactory::createWithDelta(const std::string& name, const Json::Value& node, Entity* ent)
    {
        // Possible segfaults when EntityManager reallocates and
        // the given ent pointer becomes invalid.
        // See also note in create(const Json::Value&).
        if (create(name, ent) && extendFromJson(node, *ent))
            return true;
        return false;
    }


    EntityHandle EntityFactory::create(const std::string& name)
    {
        auto found = _findTemplate(name);
        if (found)
            return createFromJson(*found);
        return EntityHandle();
    }

    bool EntityFactory::create(const std::string& name, Entity* ent)
    {
        auto found = _findTemplate(name);
        if (found)
            return createFromJson(*found, ent);
        return false;
    }

    EntityHandle EntityFactory::createFromJson(const Json::Value& node)
    {
        // NOTE: this could go wrong when the EntityManager needs to reallocate
        //       during the creation process, for example when a component
        //       creates new entities.
        //       In that case, creating entities during component initialization
        //       is not supported.
        auto entmgr = getSubsystem<EntityManager>();
        auto handle = entmgr->add();

        if (!createFromJson(node, getEntity(handle)))
        {
            entmgr->destroy(handle);
            handle.reset();
        }
        return handle;
    }

    bool EntityFactory::createFromJson(const Json::Value& node, Entity* ent)
    {
        assert(ent && "ent is null");
        // Possible segfaults when EntityManager reallocates and
        // the given ent pointer becomes invalid.
        // See also note in create(const Json::Value&).
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


    void EntityFactory::add(const Json::Value& cfg)
    {
        add(cfg["name"].asString(), cfg);
    }

    void EntityFactory::add(const std::string& name, const Json::Value& cfg)
    {
        if (name.empty())
        {
            LOG_ERROR("Entity name must not be empty");
            return;
        }

        _entdata[name] = cfg;
        LOG_DEBUG("Registered entity ", name);
    }

    void EntityFactory::addComponent(const std::string& name, ComponentFactory::CreatorFunction callback)
    {
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

    const Json::Value* EntityFactory::findEntity(const std::string& name) const
    {
        auto it = _entdata.find(name);
        if (it != _entdata.end())
            return &it->second;
        return nullptr;
    }

    auto EntityFactory::_findTemplate(const std::string& name) -> const Json::Value*
    {
        auto found = findEntity(name);
        if (!found)
        {
            auto resmgr = ResourceManager::getActive();
            if (resmgr)
            {
                // If found, the entity autoregisters at the factory.
                // Won't work if EntityFactory::getActive() returns another factory than this.
                auto handle = resmgr->getOnce(name + ".ent");
                if (!handle)
                    handle = resmgr->getOnce(name + ".entity");
                if (handle)
                    found = findEntity(name);
            }
        }
        return found;
    }
}
