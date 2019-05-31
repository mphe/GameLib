#include "gamelib/components/update/CameraTracker.hpp"
#include "gamelib/core/rendering/CameraSystem.hpp"
#include "gamelib/core/ecs/Entity.hpp"

namespace gamelib
{
    CameraTracker::CameraTracker() :
        UpdateComponent(1, UpdateHookType::PostPostFrame),
        camera(0),
        shakerad(5),
        _shake(false)
    {
        _props.registerProperty("shakeradius", shakerad);
        _props.registerProperty("camera", camera);
    }

    void CameraTracker::update(float elapsed)
    {
        auto ent = getEntity();
        if (!ent)
            return;

        Camera* cam = getCamera();
        if (!cam)
            return;

        math::Vec2f offset;

        if (_shake)
        {
            _secondsleft -= elapsed;
            if (_secondsleft <= 0)
            {
                _shake = false;
                return;
            }

            offset.fill(-shakerad);
            offset += math::Vec2f(std::fmod(random(), shakerad * 2),
                                  std::fmod(random(), shakerad * 2));
        }
        cam->center(ent->getTransform().getPosition() + offset);
    }

    void CameraTracker::shake(float seconds)
    {
        _shake = true;
        _secondsleft = seconds;
    }

    Camera* CameraTracker::getCamera() const
    {
        return getSubsystem<CameraSystem>()->get(camera);
    }
}
