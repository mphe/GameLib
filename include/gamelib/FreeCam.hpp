#ifndef GAMELIB_FREE_CAM_HPP
#define GAMELIB_FREE_CAM_HPP

#include "Updatable.hpp"

/*
 * Allows moving and zooming the camera around.
 */

namespace gamelib
{
    class Scene;

    class FreeCam : public Updatable
    {
        public:
            FreeCam(Scene* scene, float speed = 400, unsigned int camindex = 0);
            void update(float elapsed);

        public:
            unsigned int camindex;
            float speed;
            Scene* scene;
    };
}

#endif
