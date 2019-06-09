#include "gamelib/events/CameraEvents.hpp"

namespace gamelib
{
    TriggerCameraShake::TriggerCameraShake(float multiplier, float duration) :
        multiplier(multiplier),
        duration(duration)
    { }

    TriggerCameraShake::TriggerCameraShake(const std::string& camname, float multiplier, float duration) :
        camname(camname),
        multiplier(multiplier),
        duration(duration)
    { }
}
