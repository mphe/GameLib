#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/json/json-transformable.hpp"
#include "gamelib/properties/PropDummy.hpp"

namespace gamelib
{
    Component::Component() :
        _initialized(false)
    {
        registerDummyProperty(_props, "transform");
    }

    auto Component::init() -> bool
    {
        if (!_initialized)
            _initialized = _init();
        if (!_initialized)
            LOG_ERROR("Failed to initialize component ", getName());
        return _initialized;
    }

    auto Component::quit() -> void
    {
        if (_initialized)
        {
            _quit();
            _initialized = false;
        }
    }

    auto Component::isInitialized() const -> bool
    {
        return _initialized;
    }

    EntityReference Component::getEntity() const
    {
        return _entptr;
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
            gamelib::loadFromJson(node["transform"], *getTransform(), false, false);
        return _props.loadFromJson(node);
    }

    void Component::writeToJson(Json::Value& node) const
    {
        if (getTransform())
            gamelib::writeToJson(node["transform"], *getTransform(), false);
        _props.writeToJson(node);
    }
}
