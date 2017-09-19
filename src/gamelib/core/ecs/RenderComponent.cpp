#include "gamelib/core/ecs/RenderComponent.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/rendering/Scene.hpp"

namespace gamelib
{
    RenderComponent::RenderComponent(const std::string& name) :
        Identifier(name)
    { }

    bool RenderComponent::_init()
    {
        auto scene = getSubsystem<Scene>();
        if (!scene)
            return false;

        scene->add(this);
        getEntity()->getTransform().add(this);
        return true;
    }

    void RenderComponent::_quit()
    {
        getEntity()->getTransform().remove(this);
        unregister();
    }

    bool RenderComponent::loadFromJson(const Json::Value& node)
    {
        Component::loadFromJson(node);
        return SceneObject::loadFromJson(node);
    }

    void RenderComponent::writeToJson(Json::Value& node)
    {
        Component::writeToJson(node);
        SceneObject::writeToJson(node);
    }
}
