#ifndef GAMELIB_CAMERA_TRACKER_HPP
#define GAMELIB_CAMERA_TRACKER_HPP

#include "gamelib/components/UpdateComponent.hpp"

namespace gamelib
{
    class Camera;

    class CameraTracker : public UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("CameraTracker");

        public:
            CameraTracker();

            auto update(float elapsed) -> void final override;

            auto shake(float seconds) -> void;
            auto shake()              -> void;

            auto getCamera() const    -> Camera*;

        public:
            int camera;
            float shakerad;
            float shakeduration;

        private:
            float _secondsleft;
            bool _shake;
    };
}

#endif
