#include "gamelib/components/CollisionComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/geometry/CollisionSystem.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    CollisionComponent::CollisionComponent() :
        CollisionComponent("")
    { }

    CollisionComponent::CollisionComponent(const std::string& name) :
        Identifier(name)
    {
        _props.registerProperty("flags", flags, 0, num_colflags, str_colflags);
    }

    bool CollisionComponent::_init()
    {
        auto sys = getSubsystem<CollisionSystem>();
        if (!sys)
            return false;

        getEntity()->getTransform().add(this);
        sys->add(this);
        return true;
    }

    void CollisionComponent::_quit()
    {
        getSubsystem<CollisionSystem>()->remove(this);
        getEntity()->getTransform().remove(this);
    }

    Transformable* CollisionComponent::getTransform()
    {
        return this;
    }

    const Transformable* CollisionComponent::getTransform() const
    {
        return this;
    }

    bool CollisionComponent::loadFromJson(const Json::Value& node)
    {
        Component::loadFromJson(node);
        gamelib::loadFromJson(node["transform"], *static_cast<Transformable*>(this), false);
        return true;
    }

    void CollisionComponent::writeToJson(Json::Value& node)
    {
        Component::writeToJson(node);
        gamelib::writeToJson(node["transform"], *static_cast<Transformable*>(this));
    }
}
