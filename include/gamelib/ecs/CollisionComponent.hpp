#ifndef GAMELIB_COLLISIONCOMPONENT_HPP
#define GAMELIB_COLLISIONCOMPONENT_HPP

#include "gamelib/Identifier.hpp"
#include "Component.hpp"
#include "gamelib/geometry/Collidable.hpp"

namespace gamelib
{
    class CollisionComponent : public Identifier<0x87724955, Component>,
                               public Collidable
    {
        public:
            virtual ~CollisionComponent() {}

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;
    };
}

#endif
