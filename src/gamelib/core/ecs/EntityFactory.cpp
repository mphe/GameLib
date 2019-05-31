#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Entity::Handle createEntity(const std::string& name)
    {
        return getSubsystem<EntityFactory>()->create(name);
    }

    Entity::Handle createEntity(const std::string& name, float x, float y)
    {
        auto h = createEntity(name);
        getEntity(h)->getTransform().setPosition(x, y);
        return h;
    }

    EntityFactory::EntityFactory()
    { }


    Entity::Handle EntityFactory::createWithDelta(const std::string& name, const Json::Value& node)
    {
        auto handle = getSubsystem<EntityManager>()->add();
        createWithDelta(name, node, getEntity(handle));
        return handle;
    }

    void EntityFactory::createWithDelta(const std::string& name, const Json::Value& node, Entity* ent)
    {
        // Possible segfaults when EntityManager reallocates and
        // the given ent pointer becomes invalid.
        // See also note in create(const Json::Value&).
        create(name, ent);
        extendFromJson(node, *ent);
    }


    Entity::Handle EntityFactory::create(const std::string& name)
    {
        assert(_entdata.find(name) != _entdata.end());
        return createFromJson(_entdata.find(name)->second);
    }

    Entity::Handle EntityFactory::createFromJson(const Json::Value& node)
    {
        // NOTE: this could go wrong when the EntityManager needs to reallocate
        //       during the creation process, for example when a component
        //       creates new entities.
        //       In that case, creating entities during component initialization
        //       is not supported.
        auto handle = getSubsystem<EntityManager>()->add();
        createFromJson(node, getEntity(handle));
        return handle;
    }

    void EntityFactory::create(const std::string& name, Entity* ent)
    {
        assert(_entdata.find(name) != _entdata.end());
        createFromJson(_entdata.find(name)->second, ent);
    }

    void EntityFactory::createFromJson(const Json::Value& node, Entity* ent)
    {
        // Possible segfaults when EntityManager reallocates and
        // the given ent pointer becomes invalid.
        // See also note in create(const Json::Value&).
        assert(ent);
        loadFromJson(node, *ent);
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

    const Json::Value* EntityFactory::findEntity(const std::string& name)
    {
        auto it = _entdata.find(name);
        if (it != _entdata.end())
            return &it->second;
        return nullptr;
    }
}
