#include "gamelib/ecs/CollisionComponent.hpp"
#include "gamelib/ecs/Entity.hpp"
#include "gamelib/System.hpp"
#include "gamelib/geometry/CollisionSystem.hpp"
#include "gamelib/utils/json.hpp"

namespace gamelib
{
    bool CollisionComponent::_init()
    {
        auto sys = System::getActive()->getCollisionSystem();
        if (!sys)
            return false;

        sys->add(this);
        getEntity()->getTransform().add(this);
        return true;
    }

    void CollisionComponent::_quit()
    {
        getEntity()->getTransform().remove(this);
        System::getActive()->getCollisionSystem()->remove(this);
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
        gamelib::writeToJson(node["transform"], *static_cast<Transformable*>(this));
        node["flags"] = flags;
    }
}
