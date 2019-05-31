#ifndef GAMELIB_RENDER_COMPONENT_HPP
#define GAMELIB_RENDER_COMPONENT_HPP

#include "gamelib/core/Identifier.hpp"
#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/core/rendering/RenderStructs.hpp"
#include "gamelib/core/geometry/Transformable.hpp"

namespace gamelib
{
    class RenderSystem;

    class RenderComponent : public Identifier<0xa40fdfdd, Component>,
                               public Transformable
    {
        friend class PropNodeHandle;

        public:
            RenderComponent();
            virtual ~RenderComponent();

            virtual auto getTransform()       -> Transformable* override;
            virtual auto getTransform() const -> const Transformable* override;

            virtual auto getBBox() const -> math::AABBf override;

        protected:
            virtual auto _init() -> bool override;
            virtual auto _quit() -> void override;
            virtual auto _onChanged(const sf::Transform& old) -> void override;

        protected:
            NodeHandle _handle;
            RenderSystem* _system;
    };
}

#endif
