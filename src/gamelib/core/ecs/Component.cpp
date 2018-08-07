#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"

namespace gamelib
{
    Component::ComponentFactory Component::_compfactory;

    Component::Component() :
        _entptr(nullptr)
    { }

    Component::Component(const std::string& name) :
        _name(name),
        _entptr(nullptr)
    { }

    const std::string& Component::getName() const
    {
        return _name;
    }

    Entity* Component::getEntity() const
    {
        if (_entptr || _ent.isNull())
            return _entptr;
        return getSubsystem<EntityManager>()->get(_ent);
    }

    Component::Handle Component::getEntityHandle() const
    {
        return _ent;
    }

    const PropertyContainer& Component::getProperties() const
    {
        return _props;
    }

    bool Component::loadFromJson(const Json::Value& node)
    {
        return _props.loadFromJson(node);
    }

    void Component::writeToJson(Json::Value& node)
    {
        _props.writeToJson(node);
    }

    void Component::registerComponent(const std::string& name, ComponentFactory::CreatorFunction callback)
    {
        _compfactory.add(name, callback);
        LOG_DEBUG("Registered component ", name);
    }

    void Component::unregisterComponent(const std::string& name)
    {
        _compfactory.remove(name);
    }

    Component::ComponentFactory& Component::getFactory()
    {
        return _compfactory;
    }


    bool Component::_init()
    {
        return true;
    }

    void Component::_quit()
    { }

    void Component::_refresh()
    { }
}
