#include "gamelib/core/ecs/CollisionComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/geometry/CollisionSystem.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    constexpr const char* collisionFlagHints[] = { "Solid", "Mask", "Unprecise" };

    CollisionComponent::CollisionComponent() :
        CollisionComponent("")
    { }

    CollisionComponent::CollisionComponent(const std::string& name) :
        Identifier(name)
    {
        _props.registerProperty("flags", flags, 0, 3, collisionFlagHints);
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

    bool CollisionComponent::loadFromJson(const Json::Value& node)
    {
        Component::loadFromJson(node);
        gamelib::loadFromJson(node["transform"], *static_cast<Transformable*>(this));
        flags = node.get("flags", 0).asUInt();
        return true;
    }

    void CollisionComponent::writeToJson(Json::Value& node)
    {
        Component::writeToJson(node);

        if (getEntity())
            *this -= getEntity()->getTransform();

        gamelib::writeToJson(node["transform"], *static_cast<Transformable*>(this));

        if (getEntity())
            *this += getEntity()->getTransform();

        node["flags"] = flags;
    }
}
