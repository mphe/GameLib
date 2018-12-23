#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/json/json-transformable.hpp"
#include "gamelib/properties/PropDummy.hpp"

namespace gamelib
{
    Component::Component() :
        _entptr(nullptr)
    { }

    Component::Component(const std::string& name) :
        _name(name),
        _entptr(nullptr)
    {
        registerDummyProperty(_props, "transform");
    }

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

    Transformable* Component::getTransform()
    {
        return nullptr;
    }

    const Transformable* Component::getTransform() const
    {
        return nullptr;
    }

    bool Component::loadFromJson(const Json::Value& node)
    {
        if (getTransform())
            gamelib::loadFromJson(node["transform"], *getTransform(), false);
        return _props.loadFromJson(node);
    }

    void Component::writeToJson(Json::Value& node) const
    {
        if (getTransform())
            gamelib::writeToJson(node["transform"], *getTransform());
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
