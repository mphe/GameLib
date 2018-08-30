#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/core/geometry/CollisionSystem.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/components/CollisionComponent.hpp"
#include "gamelib/core/Game.hpp"

namespace gamelib
{
    // Magic Quake engine number that prevents getting stuck
    constexpr float magic_unstuck = 0.03125;

    constexpr const char* QConfig::name;

    // Statics
    math::Vec2f QPhysics::gravityDirection(0, 1);
    float QPhysics::gravity = 700;
    float QPhysics::friction = 10;
    float QPhysics::stopFriction = 10;
    float QPhysics::stopSpeed = 70;


    math::Vec2f& subclamp(math::Vec2f* vec, const math::Vec2f& other)
    {
        vec->foreach([&](float* e, size_t i) {
                *e = math::clamp(*e - other[i], std::min(*e, 0.f), std::max(*e, 0.f));
            });
        return *vec;
    }


    QConfig::QConfig() :
        Identifier(name)
    {
        _props.registerProperty("gravityDirection", QPhysics::gravityDirection);
        _props.registerProperty("gravity", QPhysics::gravity);
        _props.registerProperty("friction", QPhysics::friction);
        _props.registerProperty("stopFriction", QPhysics::stopFriction);
        _props.registerProperty("stopSpeed", QPhysics::stopSpeed);
    }



    QPhysics::QPhysics(int interval) :
        QPhysics(nullptr, interval)
    { }

    QPhysics::QPhysics(const math::AABBf* box, int interval) :
        UpdateComponent(name, interval),
        overbounce(1),
        gravMultiplier(1),
        fricMultiplier(1),
        maxSlope(0.5),
        movingPlatformSnapDist(15),
        snapDist(5),
        keepMomentum(true),
        airFriction(false),
        _bbox(box),
        _ground{ nullptr, math::Vec2f(), 0.f }
    {
        _props.registerProperty("basevel", basevel);
        _props.registerProperty("vel", vel);
        _props.registerProperty("overbounce", overbounce);
        _props.registerProperty("gravity multiplier", gravMultiplier);
        _props.registerProperty("friction multiplier", fricMultiplier);
        _props.registerProperty("maxSlope", maxSlope, 0, 1);
        _props.registerProperty("Moving snap distance", movingPlatformSnapDist);
        _props.registerProperty("Snap distance", snapDist);
        _props.registerProperty("keepMomentum", keepMomentum);
        _props.registerProperty("airFriction", airFriction);
    }


    CollisionComponent* QPhysics::moveToContact(const math::Vec2f& dist)
    {
        auto other = moveIfContact(dist);
        if (!other)
            _move(dist);
        return other;
    }

    CollisionComponent* QPhysics::moveIfContact(const math::Vec2f& dist)
    {
        TraceResult tr = getSubsystem<CollisionSystem>()->trace(*_bbox, dist, _self, collision_solid);
        if (!tr)
            return nullptr;

        if (tr.isec.time > 0) // else -> stuck
        {
            auto box = *_bbox;
            box.pos += dist * (tr.isec.time - magic_unstuck);
            if (isStuck(box))
                _nudge(&box);
            _move(box.pos - _bbox->pos);
        }
        return static_cast<CollisionComponent*>(tr.obj);
    }

    void QPhysics::clipmove(math::Vec2f* vel_)
    {
        return clipmove(vel_, getSubsystem<Game>()->getFrametime());
    }

    void QPhysics::clipmove(math::Vec2f* vel_, float elapsed)
    {
        auto colsys = getSubsystem<CollisionSystem>();
        float timeleft = elapsed;
        math::AABBf box(*_bbox);
        math::Vec2f originalvel = *vel_;
        math::Vec2f& vel = *vel_;

        // Save all intersections but use only solids for actual collisions
        std::vector<TraceResult> collisions;
        auto tracecb = [this, &collisions](Collidable* col, const Intersection& isec) {
            collisions.emplace_back(col, isec);
            return col->flags & collision_solid;
        };


        if (vel.isZero())
            return;

        while (timeleft > 0)
        {
            if (vel.isZero())
                break;

            auto framevel = vel * timeleft;
            TraceResult trace = colsys->trace(box, framevel, tracecb, _self, collision_solid | collision_physicsdrag);

            { // Move objects, flagged to be moved on collision, by current speed
                for (auto& i : collisions)
                {
                    if (trace && i.isec.time > trace.isec.time)
                        continue;
                    if (!hasflag(i.obj->flags, collision_physicsdrag))
                        continue;
                    auto phys = static_cast<CollisionComponent*>(i.obj)->getEntity()->findByName<QPhysics>();
                    if (!phys)
                        continue;

                    auto tmpvel = vel + basevel;
                    bool selfdrag = _self->flags & collision_physicsdrag;

                    // Add momentum
                    // NOTE: when an object is dragged along this will result
                    //       in jittery movement because the other object now
                    //       has additional speed, outruns the platform and gets
                    //       catched again when the friction slows it down again.
                    // NOTE: Not adding momentum provides clean dragging.
                    // TODO: could be fixed using the same snap mechanic used
                    //       for sticking to vertical moving platforms.
                    if (keepMomentum)
                    {
                        auto addvel = tmpvel;
                        subclamp(&addvel, phys->vel + phys->basevel);
                        phys->vel += addvel;
                    }

                    // Temporary unflag as physicsdrag (if needed) to prevent
                    // recursive "dragging"
                    if (selfdrag)
                        RMFLAG(_self->flags, collision_physicsdrag);
                    phys->clipmove(&tmpvel, elapsed);
                    if (selfdrag)
                        ADDFLAG(_self->flags, collision_physicsdrag);
                }
                collisions.clear();
            }

            { // Movement
                if (trace)
                {
                    if (trace.isec.time >= 0)
                    {
                        vel -= trace.isec.normal * trace.isec.normal.dot(vel) * overbounce;
                        for (size_t i = 0; i < 2; ++i)
                            if (math::inrange(vel[i], -0.2f, 0.2f))
                                vel[i] = 0;

                        box.pos += framevel * (trace.isec.time - magic_unstuck);
                        timeleft -= timeleft * trace.isec.time;

                        if (isStuck(box))
                            _nudge(&box);
                    }
                    else
                    {
                        if (isStuck(box))
                            _nudge(&box);
                        break;

                        // LOG_DEBUG("---------stuck");
                        // LOG_DEBUG("time: ", trace.isec.time);
                        // LOG_DEBUG("near: ", trace.isec.near);
                        // LOG_DEBUG("far: ", trace.isec.far);
                        // LOG_DEBUG("normal: x: ", trace.isec.normal.x, " y: ", trace.isec.normal.y);
                        // LOG_DEBUG("pos: x: ", box.x, " y: ", box.y);
                        // LOG_DEBUG("vel: x: ", vel.x, " y: ", vel.y);
                    }

                    if (vel.dot(originalvel) <= 0)
                        break;
                }
                else
                {
                    box.pos += framevel;
                    break;
                }
            }
        }

        _move(box.pos - _bbox->pos);
    }

    bool QPhysics::nudge(float size)
    {
        auto box = *_bbox;
        if (!_nudge(&box, size))
            return false;
        _move(box.pos - _bbox->pos);
        return true;
    }

    void QPhysics::applyFriction(math::Vec2f* vel_, float elapsed, bool novertical)
    {
        if (fricMultiplier == 0.f)
            return;

        auto& vel = *vel_;

        if (novertical)
        {
            auto walkdir = gravityDirection.left();
            float speedproj = vel.dot(walkdir);
            float speed = std::abs(speedproj);
            if (speed > 0)
            {
                float drop = (speed < stopSpeed) ? stopFriction : (speed * friction * fricMultiplier * elapsed);
                if (speed - drop < 0)
                    drop = speed;
                vel -= walkdir * drop * math::sign(speedproj);
            }
        }
        else
        {
            // Friction affecting every direction
            // Would also affect vertical velocity (jumping/falling), so only use
            // this when grounded
            float speed = vel.abs();
            if (speed > 0)
            {
                float drop = (speed < stopSpeed) ? stopFriction : (speed * friction * fricMultiplier * elapsed);
                vel *= std::max(speed - drop, 0.0f) / speed;
            }
        }
    }

    void QPhysics::applyFriction(math::Vec2f* vel, bool novertical)
    {
        applyFriction(vel, getSubsystem<Game>()->getFrametime(), novertical);
    }

    void QPhysics::accelerate(const math::Vec2f& wishdir, float wishspeed, float accel)
    {
        float addspeed = wishspeed - vel.dot(wishdir);

        if (addspeed <= 0)
            return;

        float accelspeed = accel * getSubsystem<Game>()->getFrametime() * wishspeed; // * groundfriction;

        if (accelspeed > addspeed)
            accelspeed = addspeed;

        vel += wishdir * accelspeed;
    }

    void QPhysics::update(float elapsed)
    {
        if (!_bbox)
            return;

        bool hasgravity = !gravityDirection.isZero() && gravMultiplier != 0.f;

        if (_state == Ground && hasgravity)
            _snapToMovingGround();
        categorizePosition();

        // Gravity
        if (hasgravity && _state == Air)  // Apply gravity
            vel += gravityDirection * (gravity * gravMultiplier * elapsed);

        // Movement
        vel += basevel;
        clipmove(&vel, elapsed);
        vel -= basevel;

        // The order of _snapToMovingGround and moveIfContact is important,
        // because when snapping to a moving ground, the vertical speed has to
        // be reset.
        if (_state == Ground && hasgravity)
            if (!_snapToMovingGround())
                moveIfContact(gravityDirection * snapDist);
        categorizePosition();

        {// Friction
            if (_state == Ground)
                applyFriction(&vel, elapsed, false);
            else if (airFriction && _state == Air)
                applyFriction(&vel, elapsed, true);
        }

        // if (isStuck())
        // {
        //     LOG_DEBUG("stuckstuck");
        //     nudge(1);
        // }
    }


    bool QPhysics::isStuck() const
    {
        return isStuck(*_bbox);
    }

    bool QPhysics::isStuck(float relx, float rely) const
    {
        auto tmp = *_bbox;
        tmp.pos += math::Vec2f(relx, rely);
        return isStuck(tmp);
    }

    bool QPhysics::isStuck(const math::AABBf& box) const
    {
        return getSubsystem<CollisionSystem>()->intersect(box, _self, collision_solid);
    }

    const math::AABBf* QPhysics::getHull() const
    {
        return _bbox;
    }

    QPhysics::State QPhysics::getState() const
    {
        return _state;
    }

    const QPhysics::GroundData& QPhysics::getGround() const
    {
        return _ground;
    }


    void QPhysics::categorizePosition()
    {
        if (isStuck())
            _state = Stuck;
        else
        {
            TraceResult tr = getSubsystem<CollisionSystem>()->trace(*_bbox, gravityDirection, _self, collision_solid);
            if (tr && (1 - std::abs(tr.isec.normal.dot(gravityDirection))) <= maxSlope)
                setGround(static_cast<CollisionComponent*>(tr.obj), tr.isec.normal);
            else
                setGround(nullptr, math::Vec2f());
        }

        { // Determine base velocity
            auto phys = _state == Ground ? _ground.hull->getEntity()->findByName<QPhysics>() : nullptr;
            if (phys)
            {
                auto base = phys->vel + phys->basevel;

                // On first contact, reduce velocity by basevelocity (as far as possible)
                if (basevel.isZero())
                    subclamp(&vel, base);
                basevel = base;
            }
            else if (!basevel.isZero())
            {
                if (keepMomentum)
                    vel += basevel;
                basevel.fill(0);
            }
        }
    }

    void QPhysics::setGround(CollisionComponent* ground, const math::Vec2f& normal)
    {
        _ground.hull = ground;
        _ground.normal = normal;
        if (ground)
        {
            _ground.slope = 1 - std::abs(normal.dot(gravityDirection));
            _state = Ground;
        }
        else
        {
            _ground.slope = 0;
            _state = Air;
        }
    }


    bool QPhysics::_nudge(math::AABBf* box, float size)
    {
        math::Vec2f alt;
        for (int y : { -1, 0, 1 })
        {
            for (int x : { -1, 0, 1 })
            {
                auto offset = math::Vec2f(x, y) * size;
                math::AABBf newbox(*box);
                newbox.pos += offset;
                if (!isStuck(newbox))
                {
                    // Prioritize nudges along x or y axis over diagonals
                    if (x == 0 ^ y == 0)
                    {
                        // LOG_DEBUG("nudged by x: ", offset.x, " y: ", offset.y);
                        box->pos = newbox.pos;
                        return true;
                    }
                    else
                        alt = offset;
                }
            }
        }

        if (!alt.isZero())
        {
            // LOG_DEBUG("nudged by x: ", alt.x, " y: ", alt.y);
            box->pos += alt;
            return true;
        }

        LOG_DEBUG("stuck");
        return false;
    }

    void QPhysics::_move(const math::Vec2f& dist)
    {
        getEntity()->getTransform().move(dist);
    }

    bool QPhysics::_snapToMovingGround()
    {
        auto colsys = getSubsystem<CollisionSystem>();
        auto dist = gravityDirection * movingPlatformSnapDist;
        TraceResult tr = colsys->trace(*_bbox, dist, _self, collision_solid);

        if (!tr || tr.isec.time <= 0)
        {
#ifndef NLOGDEBUG
            tr = colsys->trace(*_bbox, math::Vec2f(0, 1000), _self, collision_solid);
            if (tr && static_cast<CollisionComponent*>(tr.obj)->getEntity()->findByName<QPhysics>())
                LOG_DEBUG("would snap with dist: ", 1000 * tr.isec.time);
#endif
            return false;
        }

        auto phys = static_cast<CollisionComponent*>(tr.obj)->getEntity()->findByName<QPhysics>();
        if (!phys)
            return false;

        auto physvel = std::abs((phys->vel + phys->basevel).dot(gravityDirection));
        if (physvel > 0.0001)
        {
            // Allow jumps
            auto selfvel = vel.dot(gravityDirection);
            if (std::abs(selfvel) > physvel)
                return false;

            auto box = *_bbox;
            box.pos += dist * (tr.isec.time - magic_unstuck);
            if (!isStuck(box) || _nudge(&box))
            {
                vel -= gravityDirection * selfvel;
                _move(box.pos - _bbox->pos);
                // LOG_DEBUG("snapped ", 10 * tr.isec.time);
                return true;
            }
        }
        return false;
    }

    void QPhysics::_refresh()
    {
        _bbox = nullptr;
        _self = nullptr;
        getEntity()->findAllByType<CollisionComponent>([this](CollisionComponent* comp) {
                if (comp->flags & collision_physicshull)
                {
                    _bbox = &comp->getBBox();
                    _self = comp;
                    return true;
                }
                return false;
            });
    }
}
