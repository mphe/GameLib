#ifndef GAMELIB_RENDER_COMPONENT_HPP
#define GAMELIB_RENDER_COMPONENT_HPP

#include "gamelib/rendering/SceneObject.hpp"
#include "gamelib/Identifier.hpp"
#include "Component.hpp"

namespace gamelib
{
    class RenderComponent : public Identifier<0x433747bc, Component>,
                            public SceneObject
    {
        public:
            virtual ~RenderComponent() {}

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;
    };
}

#endif
