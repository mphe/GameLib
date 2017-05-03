#ifndef GAMELIB_VIM_CAM_HPP
#define GAMELIB_VIM_CAM_HPP

#include "gamelib/Updatable.hpp"

/*
 * Allows moving a camera with vim-like keybindings
 */

namespace gamelib
{
    class Scene;

    class VimCam : public Updatable
    {
        public:
            VimCam(Scene* scene, float speed = 400, unsigned int camindex = 0);
            void update(float elapsed);

        public:
            unsigned int camindex;
            float speed;
            Scene* scene;
    };
}

#endif
