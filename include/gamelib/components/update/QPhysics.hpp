#ifndef GAMELIB_QPHYSICS_HPP
#define GAMELIB_QPHYSICS_HPP

#include "math/geometry/AABB.hpp"
#include "gamelib/components/UpdateComponent.hpp"

namespace gamelib
{
    class CollisionComponent;
    class Collidable;
    class TraceResult;


    // Provides properties for global settings (gravity, friction, etc)
    class QConfig : public Identifier<0xbfae3ffa, Component>
    {
        public:
            ASSIGN_NAMETAG("QConfig");

        public:
            QConfig();
    };


    class QPhysics : public UpdateComponent
    {
        public:
            ASSIGN_NAMETAG("QPhysicsComponent");

            enum State
            {
                Air,
                Ground,
                Stuck
            };

            struct GroundData
            {
                ComponentReference<CollisionComponent> hull;
                math::Vec2f normal;
                float slope;
            };

        public:
            QPhysics(int interval = 1);
            QPhysics(Collidable* hull, int interval = 1);
            virtual ~QPhysics() {};

            virtual auto update(float elapsed) -> void override;

            // Accelerate by a given amount of units/sec in a certain direction
            // Expects wishdir to be a normalized vector.
            // Accelerates in a 1 / accel second to the desired speed.
            auto accelerate(const math::Vec2f& wishdir, float wishspeed, float accel) -> void;

            auto moveToContact(const math::Vec2f& dist)    -> ComponentReference<CollisionComponent>;
            auto moveIfContact(const math::Vec2f& dist)    -> ComponentReference<CollisionComponent>;
            auto clipmove(math::Vec2f* vel, float elapsed) -> void;
            auto clipmove(math::Vec2f* vel)                -> void;
            auto nudge(float size = 0.5)                   -> bool;
            auto applyFriction(math::Vec2f* vel, float elapsed, bool novertical = false) -> void;
            auto applyFriction(math::Vec2f* vel, bool novertical = false)                -> void;

            auto isStuck(const math::AABBf& box) const -> bool;
            auto isStuck(float relx, float rely) const -> bool;
            auto isStuck() const                       -> bool;

            auto getHull() const   -> math::AABBf;
            auto getState() const  -> State;
            auto getGround() const -> const GroundData&;

            auto categorizePosition() -> void;
            auto setGround(ComponentReference<CollisionComponent> ground, const math::Vec2f& normal) -> void;

        protected:
            virtual auto _refresh(RefreshType type, Component* comp) -> void override;

            auto _nudge(math::AABBf* box, float size = 0.5) -> bool;
            auto _move(const math::Vec2f& dist) -> void;
            auto _snapToMovingGround()          -> bool;

        public:
            static float gravity;
            static math::Vec2f gravityDirection;
            static float friction;
            static float stopFriction;
            static float stopSpeed;

        public:
            math::Vec2f vel;
            math::Vec2f basevel;
            float overbounce;
            float gravMultiplier;
            float fricMultiplier;
            float maxSlope;
            int movingPlatformSnapDist;
            int snapDist;
            bool keepMomentum;
            bool airFriction;
            // TODO: flags treated as solid

        protected:
            Collidable* _hull;    // must be non-const to temporary set/unset flags
            State _state;
            GroundData _ground;
    };

}

#endif
