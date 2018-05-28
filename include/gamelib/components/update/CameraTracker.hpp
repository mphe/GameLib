#ifndef GAMELIB_CAMERA_TRACKER_HPP
#define GAMELIB_CAMERA_TRACKER_HPP

#include "gamelib/core/ecs/UpdateComponent.hpp"
#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class Transformable;
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
            Transformable* track;
            int camera;
            float shakerad;

        private:
            float _secondsleft;
            bool _shake;
    };
}

#endif
