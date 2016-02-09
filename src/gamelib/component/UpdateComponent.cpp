#include "gamelib/component/UpdateComponent.hpp"
#include "gamelib/component/subsystem/UpdateSystem.hpp"
#include "gamelib/utils/log.hpp"
#include <cassert>

namespace gamelib
{
    UpdateComponent::UpdateComponent() : _subsys(NULL)
    { }

    UpdateComponent::~UpdateComponent()
    {
        destroy();
    }

    bool UpdateComponent::init(UpdateSystem* usystem)
    {
        _subsys = usystem;
        return true;
    }

    bool UpdateComponent::init(Subsystem* ssystem)
    {
        assert(ssystem->getID() == UpdateSystem::id && "Subsystem is not an UpdateSystem.");
        return init(static_cast<UpdateSystem*>(ssystem));
    }

    void UpdateComponent::destroy()
    {
        if (_subsys)
        {
            _subsys->remove(this);
            _subsys = NULL;
        }
    }
}
