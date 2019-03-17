#ifndef GAMELIB_COLLISIONCOMPONENT_HPP
#define GAMELIB_COLLISIONCOMPONENT_HPP

#include "gamelib/core/Identifier.hpp"
#include "gamelib/core/ecs/Component.hpp"
#include "gamelib/core/geometry/Collidable.hpp"

namespace gamelib
{
    class CollisionComponent : public Identifier<0x87724955, Component>,
                               public Collidable
    {
        public:
            CollisionComponent();
            CollisionComponent(const std::string& name);
            virtual ~CollisionComponent() {}

            virtual auto getTransform()       -> Transformable* override;
            virtual auto getTransform() const -> const Transformable* override;

        protected:
            virtual auto _init() -> bool override;
            virtual auto _quit() -> void override;
    };
}

#endif
