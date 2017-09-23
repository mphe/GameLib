#include "gamelib/components/update/CameraTracker.hpp"
#include "gamelib/core/rendering/Scene.hpp"

namespace gamelib
{
    CameraTracker::CameraTracker() :
        UpdateComponent(name),
        track(nullptr),
        camera(0),
        shakerad(5),
        _shake(false)
    { }

    void CameraTracker::update(float elapsed)
    {
        if (!track)
            return;

        Camera* cam = getCamera();

        if (_shake)
        {
            _secondsleft -= elapsed;
            if (_secondsleft <= 0)
            {
                _shake = false;
                return;
            }

            math::Vec2f off(-shakerad, -shakerad);
            off += math::Vec2f(std::fmod(random(), shakerad * 2),
                               std::fmod(random(), shakerad * 2));

            cam->center(track->getBBox().getCenter() + off);
        }
        else
        {
            cam->center(track->getBBox().getCenter());
        }
    }

    void CameraTracker::shake(float seconds)
    {
        _shake = true;
        _secondsleft = seconds;
    }

    Camera* CameraTracker::getCamera() const
    {
        return getSubsystem<Scene>()->getCamera(camera);
    }

    bool CameraTracker::loadFromJson(const Json::Value& node)
    {
        UpdateComponent::loadFromJson(node);
        shakerad = node.get("shakerad", shakerad).asFloat();
        camera = node.get("camera", 0).asInt();
        return true;
    }

    void CameraTracker::writeToJson(Json::Value& node)
    {
        UpdateComponent::writeToJson(node);
        node["shakerad"] = shakerad;
        node["camera"] = camera;
    }
}
