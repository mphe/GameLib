#ifndef GAMELIB_QPHYSICS_HPP
#define GAMELIB_QPHYSICS_HPP

#include "math/geometry/AABB.hpp"
#include "gamelib/core/ecs/UpdateComponent.hpp"

namespace gamelib
{
    class CollisionComponent;
    class Collidable;
    class TraceResult;


    // Provides properties for global settings (gravity, friction, etc)
    class QConfig : public Identifier<0xbfae3ffa, Component>
    {
        public:
            constexpr static const char* name = "QConfig";

        public:
            QConfig();
    };


    class QPhysics : public UpdateComponent
    {
        public:
            constexpr static const char* name = "QPhysicsComponent";

            enum UnstuckMethod
            {
                Normal,
                Upwards,
                Nudge   // TODO: unimplemented
            };

        public:
            QPhysics(int interval = 1);
            QPhysics(const math::AABBf* box, int interval = 1);
            virtual ~QPhysics() {};

            virtual auto update(float elapsed) -> void;

            // Accelerate by a given amount of units/sec in a certain direction
            // Expects wishdir to be a normalized vector.
            auto accelerate(const math::Vec2f& wishdir, float wishspeed, float accel) -> void;

            auto getHull() -> const math::AABBf*;

        protected:
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
            UnstuckMethod unstuckmethod;

        protected:
            const math::AABBf* _bbox;
            const Collidable* _self;
    };

}

#endif
