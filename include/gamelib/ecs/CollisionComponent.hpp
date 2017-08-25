#ifndef GAMELIB_COLLISIONCOMPONENT_HPP
#define GAMELIB_COLLISIONCOMPONENT_HPP

#include "gamelib/Identifier.hpp"
#include "Component.hpp"
#include "gamelib/collision/Collidable.hpp"

namespace gamelib
{
    // TODO: try to merge Collidable and CollisionComponent so there is a similar relationship as
    //       for SceneObject and RenderComponent.
    //       This Wrapper workaround is not particulary nice.

    class CollisionComponent : public Identifier<0x87724955, Component>
    {
        public:
            virtual ~CollisionComponent() {}

            virtual auto get()       -> Collidable* = 0;
            virtual auto get() const -> const Collidable* = 0;

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;
    };

    template <typename T>
    class WrappedCollisionComponent : public CollisionComponent
    {
        static_assert(std::is_base_of<gamelib::Collidable, T>::value, "Not derived from gamelib::Collidable");

        public:
            template <typename... Args>
            WrappedCollisionComponent(Args&&... args) : _collidable(std::forward<Args>(args)...) {}
            WrappedCollisionComponent(const WrappedCollisionComponent&) = default;
            WrappedCollisionComponent(WrappedCollisionComponent&&) = default;
            virtual ~WrappedCollisionComponent () {}

            Collidable* get()
            {
                return &_collidable;
            }

            const Collidable* get() const
            {
                return &_collidable;
            }

        protected:
            T _collidable;
    };
}

#endif
