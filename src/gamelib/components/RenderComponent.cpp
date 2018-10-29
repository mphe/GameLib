#include "gamelib/components/RenderComponent.hpp"
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
        return true;
    }

    void RenderComponent::_quit()
    {
        unregister();
    }

    Transformable* RenderComponent::getTransform()
    {
        return this;
    }

    const Transformable* RenderComponent::getTransform() const
    {
        return this;
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
