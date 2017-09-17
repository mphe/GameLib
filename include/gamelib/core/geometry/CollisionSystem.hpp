#ifndef GAMELIB_COLLISION_SYSTEM_HPP
#define GAMELIB_COLLISION_SYSTEM_HPP

#include <vector>
#include "math/geometry/Line2.hpp"
#include "gamelib/core/Subsystem.hpp"
#include "Collidable.hpp"

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

            // Returns the colliding object if there is a collison at the
            // given point/rect, otherwise nullptr.
            auto find(const math::Point2f& point, unsigned int flags = 0) const -> Collidable*;

            // Calls a function for each colliding object at the given point.
            // Signature: bool(Collidable*)
            // If the function returns true, the loop will break.
            template <typename F>
            auto findAll(const math::Point2f& point, unsigned int flags, F f) const -> void
            {
                // Use rbegin so it conforms with find().
                for (auto it = _objs.rbegin(), end = _objs.rend(); it != end; ++it)
                {
                    Collidable* i = (*it);
                    if ((!flags || i->flags & flags) && i->intersect(point))
                        if (f(i))
                            return;
                }
            }

        private:
            std::vector<Collidable*> _objs;
    };
}

#endif
