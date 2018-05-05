#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"

namespace gamelib
{
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

    bool Component::loadFromJson(const Json::Value& node)
    {
        return _props.loadFromJson(node);
    }

    void Component::writeToJson(Json::Value& node)
    {
        node["name"] = getName();
        _props.writeToJson(node);
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
