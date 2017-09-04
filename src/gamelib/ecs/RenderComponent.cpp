#include "gamelib/ecs/RenderComponent.hpp"
#include "gamelib/System.hpp"
#include "gamelib/rendering/Scene.hpp"

namespace gamelib
{
    RenderComponent::RenderComponent(const std::string& name) :
        Identifier(name)
    { }

    bool RenderComponent::_init()
    {
        auto scene = System::getActive()->getScene();
        if (!scene)
            return false;

        scene->add(this);
        return true;
    }

    void RenderComponent::_quit()
    {
        unregister();
    }

    bool RenderComponent::loadFromJson(const Json::Value& node)
    {
        Component::loadFromJson(node);

        // TODO: layer
        setParallax(node.get("parallax", 1).asFloat());
        setDepth(node.get("depth", 0).asInt());
        flags = node.get("flags", 0).asUInt();
        return true;
    }

    void RenderComponent::writeToJson(Json::Value& node)
    {
        Component::writeToJson(node);

        // TODO: give layers a unique id
        node["layer"] = getLayer().index;
        node["parallax"] = getParallax();
        node["depth"] = getDepth();
        node["flags"] = flags;
    }
}
