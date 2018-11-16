#ifndef GAMELIB_RENDER_COMPONENT_HPP
#define GAMELIB_RENDER_COMPONENT_HPP

#include "gamelib/core/rendering/SceneObject.hpp"
#include "gamelib/core/Identifier.hpp"
#include "gamelib/core/ecs/Component.hpp"

namespace gamelib
{
    class RenderComponent : public Identifier<0x433747bc, Component>,
                            public SceneObject
    {
        public:
            RenderComponent() {}
            RenderComponent(const std::string& name);
            virtual ~RenderComponent() {}

            virtual auto getTransform()       -> Transformable*;
            virtual auto getTransform() const -> const Transformable*;

            using Component::loadFromJson;
            using Component::writeToJson;

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;
    };
}

#endif
