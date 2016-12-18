#ifndef GAMELIB_COLLISION_SYSTEM_HPP
#define GAMELIB_COLLISION_SYSTEM_HPP

#include <map> // TODO: not unordered_map ?
#include <vector>
#include "math/geometry/Vector.hpp"
#include "math/geometry/AABB.hpp"
#include "Collidable.hpp"
#include "gamelib/Identifiable.hpp"

// TODO: Use quadtrees

// The CollisionSystem class keeps track of Collidable-based objects.
// It doesn't manage the objects' lifetime, it only takes pointers to Collidables.
// Allocating and freeing objects is up to the user.
// It provides an easy to use collision system by defining methods for common collision checks.
// To register an object call add() and to unregister remove().

namespace gamelib
{
    class Collidable;

    class CollisionSystem
    {
        public:
            void add(Collidable* col, ID id);
            void remove(Collidable* col, ID id);
            void destroy();
            size_t size() const;

            // Returns the colliding object if there is a collison at the given point/rect, otherwise NULL.
            Collidable* contains(const math::Vec2f& point) const;
            Collidable* intersects(const math::AABBf& rect) const;
            Collidable* contains(const math::Vec2f& point, ID id) const;
            Collidable* intersects(const math::AABBf& rect, ID id) const;

        private:
            std::map<ID, std::vector<Collidable*> > _objs;
    };
}

#endif
