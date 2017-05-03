#ifndef GAMELIB_SCENE_OBJECT_HPP
#define GAMELIB_SCENE_OBJECT_HPP

#include "gamelib/Renderable.hpp"

namespace gamelib
{
    class Scene;

    class SceneObject : public Renderable
    {
        friend class Scene;

        public:
            SceneObject(int depth = 0, unsigned int flags = 0);
            virtual ~SceneObject() {}

            auto setDepth(int depth) -> void;
            auto getDepth() const    -> int;

            auto unregister() -> void;

        protected:
            unsigned int _flags;
            int _depth;
            Scene* _scene;
    };
}


#endif
