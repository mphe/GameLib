#ifndef GAMELIB_RENDER_COMPONENT_HPP
#define GAMELIB_RENDER_COMPONENT_HPP

#include "gamelib/core/rendering/SceneObject.hpp"
#include "gamelib/core/Identifier.hpp"
#include "Component.hpp"

namespace gamelib
{
    class RenderComponent : public Identifier<0x433747bc, Component>,
                            public SceneObject
    {
        public:
            RenderComponent() {}
            RenderComponent(const std::string& name);
            virtual ~RenderComponent() {}

            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;
    };
}

#endif
