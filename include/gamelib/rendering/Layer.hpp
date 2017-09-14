#ifndef GAMELIB_SCENE_LAYER_HPP
#define GAMELIB_SCENE_LAYER_HPP

#include "SceneData.hpp"
#include "gamelib/utils/SlotMap.hpp"

namespace gamelib
{
    class Layer : public SceneData
    {
        friend class Scene;

        public:
            typedef SlotKeyShort Handle;

        public:
            Layer(int depth = 0, float parallax = 1, unsigned int flags = 0);

            auto getUniqueID() const -> size_t;

        private:
            size_t _id; // unique id to identify layers when loading configs
    };
}

#endif
