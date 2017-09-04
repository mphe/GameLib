#include "gamelib/ecs/EntityFactory.hpp"
#include "gamelib/ecs/EntityManager.hpp"
#include "gamelib/System.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    EntityFactory::EntityFactory()
    { }

    Entity::Handle EntityFactory::create(const std::string& name)
    {
        assert(_entdata.find(name) != _entdata.end());
        return createFromJson(*_entdata.find(name)->second);
    }

    Entity::Handle EntityFactory::createFromJson(const Json::Value& node)
    {
        // NOTE: this could go wrong when the EntityManager needs to reallocate
        //       during the creation process, for example when a component
        //       creates new entities.
        //       In that case, creating entities during component initialization
        //       is not supported.
        auto handle = System::getActive()->getEntityManager()->add();
        createFromJson(node, getEntity(handle));
        return handle;
    }

    void EntityFactory::create(const std::string& name, Entity* ent)
    {
        assert(ent);
        assert(_entdata.find(name) != _entdata.end());
        createFromJson(*_entdata.find(name)->second, ent);
    }

    void EntityFactory::createFromJson(const Json::Value& node, Entity* ent)
    {
        assert(ent);
        ent->destroy();
        ent->loadFromJson(node);

        if (node.isMember("components"))
        {
            auto& cnode = node["components"];
            for (Json::ArrayIndex i = 0; i < cnode.size(); ++i)
            {
                auto comp = createComponentFromJson(cnode[i]);
                if (comp)
                    // Possible segfaults when EntityManager reallocates and
                    // the given ent pointer became invalid.
                    // See also note in create(const Json::Value&).
                    ent->add(std::move(comp));
            }
        }
    }

    ComponentPtr EntityFactory::createComponent(const std::string& name)
    {
        return _compfactory.create(name);
    }

    ComponentPtr EntityFactory::createComponentFromJson(const Json::Value& node)
    {
        assert(node.isMember("name"));
        auto ptr = createComponent(node["name"].asString());
        if (ptr && ptr->loadFromJson(node))
            return ptr;
        LOG_ERROR("Failed to create component ", node["name"].asString());
        return nullptr;
    }


    void EntityFactory::add(JsonResource::Handle res)
    {
        _entdata[(*res.get())["name"].asString()] = res;
    }

    void EntityFactory::add(const std::string& name, JsonResource::Handle res)
    {
        _entdata[name] = res;
    }

    void EntityFactory::addComponent(const std::string& name, ComponentFactory::CreatorFunction callback)
    {
        _compfactory.add(name, callback);
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
}
