#ifndef GAMELIB_SCENE_OBJECT_HPP
#define GAMELIB_SCENE_OBJECT_HPP

#include "gamelib/Renderable.hpp"
#include "Layer.hpp"

namespace gamelib
{
    class SceneObject : public Renderable, public Layer
    {
        friend class Scene;

        public:
            SceneObject(int depth = 0, unsigned int flags = 0);
            virtual ~SceneObject() {}

            auto setLayer(Layer::Handle layer) -> void;
            auto getLayer() const              -> Layer::Handle;

            auto unregister() -> void;

        protected:
            Layer::Handle _layer;   // TODO: Could be turned into a parent when Scene supports it
    };
}


#endif
