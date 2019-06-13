#ifndef GAMELIB_CAMERAEVENTS_HPP
#define GAMELIB_CAMERAEVENTS_HPP

#include "gamelib/core/event/Event.hpp"
#include <string>

namespace gamelib
{
    class TriggerCameraShake : public Event<0xa72eb67d, TriggerCameraShake>
    {
        public:
            TriggerCameraShake(float multiplier = 1, float duration = -1);
            TriggerCameraShake(const std::string& camname, float multiplier = 1, float duration = -1);

            std::string camname;
            float multiplier;
            float duration;
    };
}

#endif
