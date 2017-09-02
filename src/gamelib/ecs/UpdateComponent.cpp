#include "gamelib/ecs/UpdateComponent.hpp"
#include "gamelib/System.hpp"
#include "gamelib/update/UpdateSystem.hpp"

namespace gamelib
{
    UpdateComponent::UpdateComponent(int interval) :
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
}
