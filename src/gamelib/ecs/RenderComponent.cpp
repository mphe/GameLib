#include "gamelib/ecs/RenderComponent.hpp"
#include "gamelib/System.hpp"
#include "gamelib/rendering/Scene.hpp"

namespace gamelib
{
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
}
