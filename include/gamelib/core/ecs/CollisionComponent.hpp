#ifndef GAMELIB_COLLISIONCOMPONENT_HPP
#define GAMELIB_COLLISIONCOMPONENT_HPP

#include "gamelib/core/Identifier.hpp"
#include "Component.hpp"
#include "gamelib/core/geometry/Collidable.hpp"

namespace gamelib
{
    class CollisionComponent : public Identifier<0x87724955, Component>,
                               public Collidable
    {
        public:
            CollisionComponent() {};
            CollisionComponent(const std::string& name);
            virtual ~CollisionComponent() {}

            virtual auto loadFromJson(const Json::Value& node) -> bool;
            virtual auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;
    };
}

#endif
