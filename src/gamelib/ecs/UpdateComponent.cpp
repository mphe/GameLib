#include "gamelib/ecs/UpdateComponent.hpp"

namespace gamelib
{
    UpdateComponent::UpdateComponent(int interval, UpdateSystem::HookType hook) :
        Updatable(interval),
        _hook(hook)
    { }

    UpdateComponent::UpdateComponent(const std::string& name, int interval, UpdateSystem::HookType hook) :
        Identifier(name),
        Updatable(interval),
        _hook(hook)
    { }

    bool UpdateComponent::_init()
    {
        auto sys = getSubsystem<UpdateSystem>();
        if (!sys)
            return false;

        _handle = sys->add(this, _hook);
        return true;
    }

    void UpdateComponent::_quit()
    {
        auto sys = getSubsystem<UpdateSystem>();
        if (sys)
            sys->remove(_handle, _hook);
    }

    bool UpdateComponent::loadFromJson(const Json::Value& node)
    {
        Component::loadFromJson(node);
        interval = node.get("interval", 1).asInt();

        auto hook = static_cast<UpdateSystem::HookType>(node.get("hook", UpdateSystem::Frame).asInt());
        if (!_handle.isNull() && _hook != hook)
        {
            _quit();
            _hook = hook;
            return _init();
        }
        else
            _hook = hook;

        return true;
    }

    void UpdateComponent::writeToJson(Json::Value& node)
    {
        Component::writeToJson(node);
        node["interval"] = interval;
        node["hook"] = _hook;
    }
}
