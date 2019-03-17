#ifndef GAMELIB_FREE_CAM_HPP
#define GAMELIB_FREE_CAM_HPP

#include "gamelib/core/update/Updatable.hpp"

/*
 * Allows moving and zooming the camera around.
 */

namespace gamelib
{
    class Camera;

    class FreeCamController : public Updatable
    {
        public:
            FreeCamController(Camera* cam, float speed = 400);

            void update(float elapsed) final override;

        public:
            Camera* cam;
            float speed;
            bool freeze;
    };
}

#endif
