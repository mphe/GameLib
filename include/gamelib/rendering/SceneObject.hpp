#ifndef GAMELIB_SCENE_OBJECT_HPP
#define GAMELIB_SCENE_OBJECT_HPP

#include "gamelib/Renderable.hpp"
#include "SceneData.hpp"
#include "Layer.hpp"

namespace gamelib
{
    class SceneObject : public Renderable, public SceneData
    {
        public:
            SceneObject(int depth = 0, float parallax = 1, unsigned int flags = 0);
            virtual ~SceneObject() {}

            auto setLayer(Layer::Handle layer) -> void;
            auto getLayer() const              -> Layer::Handle;

            // Returns the flags merged with the layer's and scene's flags
            auto getAllFlags() const -> unsigned int;
            auto isVisible() const   -> bool;

            auto unregister() -> void;

            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

        protected:
            Layer::Handle _layer;   // TODO: Could be turned into a parent when Scene supports it
    };
}


#endif
