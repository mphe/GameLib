#ifndef GAMELIB_RENDER_COMPONENT_HPP
#define GAMELIB_RENDER_COMPONENT_HPP

#include "gamelib/utils/Identifier.hpp"
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

            auto setDepth(int depth) -> void;
            auto getDepth() const    -> int;

            auto setLayer(LayerHandle layer) -> void;
            auto getLayer() const            -> LayerHandle;

        protected:
            virtual auto _init() -> bool override;
            virtual auto _quit() -> void override;
            virtual auto _onChanged(const sf::Transform& old) -> void override;

            auto _getNode() const -> const RenderNode*;

        protected:
            NodeHandle _handle;
            RenderSystem* _system;
    };
}

#endif
