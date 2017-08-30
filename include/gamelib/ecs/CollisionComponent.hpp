#ifndef GAMELIB_COLLISIONCOMPONENT_HPP
#define GAMELIB_COLLISIONCOMPONENT_HPP

#include "gamelib/Identifier.hpp"
#include "Component.hpp"
#include "gamelib/collision/Collidable.hpp"

namespace gamelib
{
    class CollisionComponent : public Identifier<0x87724955, Component>,
                               public Collidable
    {
        public:
            virtual ~CollisionComponent() {}

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;
    };
}

#endif
