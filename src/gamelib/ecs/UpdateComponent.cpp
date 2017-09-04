#include "gamelib/ecs/UpdateComponent.hpp"
#include "gamelib/System.hpp"
#include "gamelib/update/UpdateSystem.hpp"

namespace gamelib
{
    UpdateComponent::UpdateComponent(int interval) :
        Updatable(interval)
    { }

    UpdateComponent::UpdateComponent(const std::string& name, int interval) :
        Identifier(name),
        Updatable(interval)
    { }

    bool UpdateComponent::_init()
    {
        auto sys = System::getActive()->getUpdateSystem();
        if (!sys)
            return false;

        _handle = sys->add(this);
        return true;
    }

    void UpdateComponent::_quit()
    {
        auto sys = System::getActive()->getUpdateSystem();
        if (sys)
            sys->remove(_handle);
    }

    bool UpdateComponent::loadFromJson(const Json::Value& node)
    {
        Component::loadFromJson(node);
        interval = node.get("interval", 1).asInt();
        return true;
    }

    void UpdateComponent::writeToJson(Json::Value& node)
    {
        Component::writeToJson(node);
        node["interval"] = interval;
    }
}
