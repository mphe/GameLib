#ifndef GAMELIB_FREE_CAM_HPP
#define GAMELIB_FREE_CAM_HPP

#include "Camera.hpp"

/*
 * Allows moving and zooming the camera around.
 */

namespace gamelib
{
    class FreeCam : public Camera
    {
        public:
            FreeCam(const std::string& name, float speed = 400);
            FreeCam(const std::string& name, const math::Vec2f& pos, const math::Vec2f& size, float speed = 400);
            virtual ~FreeCam() {}

            virtual void update(float elapsed);

        public:
            float speed;
            bool freeze;
    };
}

#endif
