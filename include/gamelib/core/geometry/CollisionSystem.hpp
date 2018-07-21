#ifndef GAMELIB_COLLISION_SYSTEM_HPP
#define GAMELIB_COLLISION_SYSTEM_HPP

#include <vector>
#include "math/geometry/Line2.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "Collidable.hpp"
#include "flags.hpp"

// TODO: Use quadtrees

// The CollisionSystem class keeps track of Collidable-based objects.
// It doesn't manage the objects' lifetime, it only takes pointers to
// Collidables. Allocating and freeing objects is up to the user.
// To register an object call add() and to unregister remove().

namespace gamelib
{
    class Collidable;

    class TraceResult
    {
        public:
            TraceResult();
            TraceResult(Collidable* col, const Intersection& isec);
            operator bool() const;

        public:
            Collidable* obj;    // TODO: maybe make this a CollisionComponent
            Intersection isec;
    };

    class CollisionSystem : public Subsystem<CollisionSystem>
    {
        public:
            constexpr static const char* name = "CollisionSystem";

        public:
            auto add(Collidable* col)    -> void;
            auto remove(Collidable* col) -> void;
            auto destroy()               -> void;
            auto size() const            -> size_t;

            auto trace(const math::Line2f& line,
                    const Collidable* self = nullptr, unsigned int flags = 0) const -> TraceResult;

            auto trace(const math::AABBf& rect, const math::Vec2f& vel,
                    const Collidable* self = nullptr, unsigned int flags = 0) const -> TraceResult;

            // Same as normal trace but calls a filter function for each found object.
            // Signature: bool(Collidable*, const Intersection&)
            // If the function returns false, the object will be skipped
            template <typename F>
            auto trace(const math::Line2f& line, F callback,
                    const Collidable* self = nullptr, unsigned int flags = 0) const -> TraceResult;

            template <typename F>
            auto trace(const math::AABBf& rect, const math::Vec2f& vel, F callback,
                    const Collidable* self = nullptr, unsigned int flags = 0) const -> TraceResult;

            // Returns the colliding object if there is a collison at the
            // given point/rect, otherwise nullptr.
            auto intersect(const math::Point2f& point, const Collidable* self = nullptr, unsigned int flags = 0) const -> Collidable*;
            auto intersect(const math::AABBf& rect, const Collidable* self = nullptr, unsigned int flags = 0) const    -> Collidable*;

            // Calls a function for each colliding object at the given point/rect.
            // Signature: bool(Collidable*)
            // If the function returns true, the loop will break and return that object.
            template <typename F>
            auto intersectAll(const math::Point2f& point, const Collidable* self, unsigned int flags, F f) const -> Collidable*;

            template <typename F>
            auto intersectAll(const math::AABBf& rect, const Collidable* self, unsigned int flags, F f) const -> Collidable*;

        private:
            template <typename Shape, typename F>
            auto _intersectAll(const Shape& shape, F f, const Collidable* self, unsigned int flags) const -> Collidable*;

        private:
            std::vector<Collidable*> _objs;
    };

    template <typename Shape, typename F>
    Collidable* CollisionSystem::_intersectAll(const Shape& shape, F f, const Collidable* self, unsigned int flags) const
    {
        // Use rbegin so that newly added object are "on top"
        for (auto it = _objs.rbegin(), end = _objs.rend(); it != end; ++it)
        {
            Collidable* c = (*it);
            if (c != self && (!flags || c->flags & flags))
            {
                if (c->flags & collision_noprecise)
                {
                    if (!c->getBBox().intersect(shape))
                        continue;
                }
                else
                {
                    if (!c->intersect(shape))
                        continue;
                }

                if (f(c))
                    return c;
            }
        }
        return nullptr;
    }

    template <typename F>
    Collidable* CollisionSystem::intersectAll(const math::Point2f& point, const Collidable* self, unsigned int flags, F f) const
    {
        return _intersectAll(point, f, self, flags);
    }

    template <typename F>
    Collidable* CollisionSystem::intersectAll(const math::AABBf& rect, const Collidable* self, unsigned int flags, F f) const
    {
        return _intersectAll(rect, f, self, flags);
    }

    template <typename F>
    TraceResult CollisionSystem::trace(const math::Line2f& line, F callback, const Collidable* self, unsigned int flags) const
    {
        TraceResult nearest;
        for (auto it = _objs.rbegin(), end = _objs.rend(); it != end; ++it)
        {
            Collidable* i = (*it);
            if (i != self && (!flags || i->flags & flags))
            {
                Intersection isec;
                if (i->flags & collision_noprecise)
                    isec = line.intersect(i->getBBox());
                else
                {
                    isec = i->intersect(line);
                    if (isec && isec.type == math::LinexLine)
                        std::swap(isec.near, isec.far);
                }

                if (isec && callback(i, isec) && (!nearest || isec.near < nearest.isec.near))
                {
                    nearest.obj = i;
                    nearest.isec = isec;
                }
            }
        }
        return nearest;
    }

    template <typename F>
    TraceResult CollisionSystem::trace(const math::AABBf& rect, const math::Vec2f& vel, F callback,
            const Collidable* self, unsigned int flags) const
    {
        TraceResult nearest;
        for (auto it = _objs.rbegin(), end = _objs.rend(); it != end; ++it)
        {
            Collidable* i = (*it);
            if (i != self && (!flags || i->flags & flags))
            {
                Intersection isec;
                if (i->flags & collision_noprecise)
                    isec = rect.sweep(vel, i->getBBox());
                else
                    isec = i->sweep(rect, vel);

                if (isec && callback(i, isec) && (!nearest || isec.near < nearest.isec.near))
                {
                    nearest.obj = i;
                    nearest.isec = isec;
                }
            }
        }
        return nearest;
    }
}

#endif
