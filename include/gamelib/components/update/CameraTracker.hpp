#ifndef GAMELIB_CAMERA_TRACKER_HPP
#define GAMELIB_CAMERA_TRACKER_HPP

#include "gamelib/components/UpdateComponent.hpp"

namespace gamelib
{
    class Camera;

    class CameraTracker : public UpdateComponent
    {
        public:
            constexpr static const char* name = "CameraTracker";

        public:
            CameraTracker();

            auto update(float elapsed) -> void;

            auto shake(float seconds) -> void;
            auto getCamera() const    -> Camera*;

        public:
            int camera;
            float shakerad;

        private:
            float _secondsleft;
            bool _shake;
    };
}

#endif
