#ifndef GAMELIB_SCENE_LAYER_HPP
#define GAMELIB_SCENE_LAYER_HPP

#include "gamelib/utils/SlotMap.hpp"

namespace gamelib
{
    class Scene;

    class Layer
    {
        friend class Scene;

        public:
            typedef SlotKeyShort Handle;

        public:
            Layer(int depth = 0, float parallax = 1, unsigned int flags = 0);
            virtual ~Layer() {}

            auto setDepth(int depth) -> void;
            auto getDepth() const    -> int;

            auto setParallax(float parallax) -> void;
            auto getParallax() const         -> float;

        public:
            unsigned int flags;

        protected:
            Scene* _scene;  // Set by Scene

        private:
            float _parallax;
            int _depth;
    };
}

#endif
