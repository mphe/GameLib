#include "gamelib/components/UpdateComponent.hpp"

namespace gamelib
{
    UpdateComponent::UpdateComponent(const std::string& name, int interval_, UpdateHookType hook) :
        Identifier(name),
        interval(interval_),
        _hook(hook)
    {
        _props.registerProperty("interval", interval);
        _props.registerProperty("hook", _hook, PROP_METHOD(_hook, setHook), this, 0, NumFrameHooks, str_framehooks);
    }

    UpdateComponent::~UpdateComponent()
    {
        quit();
    }

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

    void UpdateComponent::setHook(UpdateHookType hook)
    {
        if (!_handle.isNull() && _hook != hook)
        {
            _quit();
            _hook = hook;
            _init();
        }
        else
            _hook = hook;
    }

    UpdateHookType UpdateComponent::getHook() const
    {
        return _hook;
    }
}
