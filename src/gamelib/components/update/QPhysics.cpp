#include "gamelib/components/update/QPhysics.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/core/geometry/CollisionSystem.hpp"
#include "gamelib/core/geometry/flags.hpp"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/CollisionComponent.hpp"
#include "gamelib/core/Game.hpp"

namespace gamelib
{
    constexpr const char* unstuckmethodHints[] = { "Normal", "Upwards", "Nudge (unimplemented)" };

    constexpr const char* QConfig::name; // Why?

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
        QPhysics(nullptr, 1)
    { }

    QPhysics::QPhysics(const math::AABBf* box, int interval) :
        UpdateComponent(name, interval),
        overbounce(1),
        ground(nullptr),
        unstuckmethod(Normal),
        _bbox(box)
    {
        _props.registerProperty("vel", vel);
        _props.registerProperty("overbounce", overbounce);
        _props.registerProperty("unstuckMethod", *(int*)(&unstuckmethod), 0, 3, unstuckmethodHints);
    }

    void QPhysics::accelerate(const math::Vec2f& wishdir, float wishspeed, float accel)
    {
        float addspeed = wishspeed - vel.dot(wishdir);

        if (addspeed <= 0)
            return;

        float accelspeed = accel * getSubsystem<Game>()->getFrametime() * wishspeed * friction;

        if (accelspeed > addspeed)
            accelspeed = addspeed;

        vel += wishdir * accelspeed;
    }

    void QPhysics::update(float elapsed)
    {
        if (!_bbox)
            return;

        _checkGround();

        if (!ground)   // Apply gravity
        {
            vel += gravityDirection * (gravity * elapsed);
        }
        else    // Apply friction
        {
            float speed = vel.abs();
            if (speed > 0)
            {
                float drop = (speed < stopSpeed) ? stopFriction : (speed * friction * elapsed);
                vel *= std::max(speed - drop, 0.0f) / speed;
            }
        }

        // TODO: basevelocity
        _clipmove(elapsed);
    }


    void QPhysics::_checkGround()
    {
        TraceResult tr = getSubsystem<CollisionSystem>()->trace(
                *_bbox, gravityDirection, _self, collision_solid);
        ground = static_cast<CollisionComponent*>(tr.obj);
    }

    void QPhysics::_clipmove(float elapsed)
    {
        // auto newpos = _bbox->pos;
        auto originalvel = vel;
        float timeleft = elapsed;
        TraceResult trace;
        math::AABBf box(*_bbox);

        while (timeleft > 0)
        {
            if (vel == math::Vec2f())
                return;

            auto framevel = vel * timeleft;
            trace = getSubsystem<CollisionSystem>()->trace(box, framevel, nullptr, collision_solid);

            if (trace)
            {
                if (trace.isec.time > 0)
                {
                    vel = originalvel - trace.isec.normal * trace.isec.normal.dot(vel) * overbounce;
                    for (size_t i = 0; i < 2; ++i)
                        if (vel[i] < 0.2 && vel[i] >= -0.2)
                            vel[i] = 0;

                    // Magic Quake engine number that prevents getting stuck
                    box.pos += framevel * (trace.isec.time - 0.03125);
                    timeleft -= timeleft * trace.isec.time;
                }
                else
                {
                    LOG_DEBUG("stuck");
                    LOG_DEBUG("normal: x: ", trace.isec.normal.x, " y: ", trace.isec.normal.y);

                    if (unstuckmethod == Normal)
                    {
                        box.pos += trace.isec.normal;
                        break;
                    }
                    else if (unstuckmethod == Upwards)
                    {
                        box.pos.y -= 0.03125;
                        timeleft -= timeleft * (0.03125 / framevel.abs());
                    }
                    // _nudge(&newpos, trace);
                }

                if (vel.dot(originalvel) <= 0)
                    break;
            }
            else
            {
                // Whole distance covered
                box.pos += framevel;
                break;
            }
        }

        getEntity()->getTransform().move(box.pos - _bbox->pos);
    }

    void QPhysics::_nudge(math::Vec2f* newpos, const TraceResult& trace)
    {
        // for (int y : { -1, 0, 1 })
        //     for (int x : { -1, 0, 1 })
        //     {
        //         math::AABBf(*newpos, _bbox)
        //         if (getSubsystem<CollisionSystem>()->trace(*_bbox, math::Vec2f(x, y) * 0.125, nullptr, collision_solid))
        //             // if (!trace.obj->intersect(*newpos + math::Vec2f(x, y) * 0.125))
        //         {
        //             *newpos += math::Vec2f(x, y);
        //             return;
        //         }
        //     }
    }

    void QPhysics::_refresh()
    {
        auto comp = getEntity()->findByType<CollisionComponent>();
        if (comp)
        {
            _bbox = &comp->getBBox();
            _self = comp;
        }
    }


    math::Vec2f QPhysics::gravityDirection(0, 1);
    float QPhysics::gravity = 3000;
    float QPhysics::friction = 0.5;
    float QPhysics::stopFriction = 10;
    float QPhysics::stopSpeed = 100;
}
