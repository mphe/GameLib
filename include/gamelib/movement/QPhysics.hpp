#ifndef GAMELIB_QPHYSICS_HPP
#define GAMELIB_QPHYSICS_HPP

#include "math/geometry/AABB.hpp"
#include "gamelib/ecs/UpdateComponent.hpp"
// #include "gamelib/geometry/Transformable.hpp"

namespace gamelib
{
    class CollisionComponent;
    class Collidable;
    class TraceResult;

    class QPhysics : public UpdateComponent
    {
        public:
            constexpr static const char* name = "QPhysicsComponent";

        public:
            QPhysics(int interval = 1);
            QPhysics(const math::AABBf* box, int interval = 1);
            virtual ~QPhysics() {};

            virtual auto update(float elapsed) -> void;

            // auto move(const math::Vec2f& rel) -> void;
            // auto getPosition() const          -> const math::Point2f&;
            // auto getBBox() const              -> const math::AABBf&;

            // Accelerate by a given amount of units/sec in a certain direction
            // Expects wishdir to be a normalized vector.
            auto accelerate(const math::Vec2f& wishdir, float wishspeed, float accel) -> void;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

        protected:
            virtual auto _init() -> bool;
            virtual auto _quit() -> void;
            virtual auto _refresh() -> void;

            auto _checkGround()           -> void;
            auto _clipmove(float elapsed) -> void;
            auto _nudge(math::Vec2f* newpos, const TraceResult& trace) -> void;

        public:
            static float gravity;
            static math::Vec2f gravityDirection;
            static float friction;
            static float stopFriction;
            static float stopSpeed;

        public:
            math::Vec2f vel;
            float overbounce;
            CollisionComponent* ground;

        protected:
            const math::AABBf* _bbox;
            const Collidable* _self;
    };
}

#endif
