#ifndef GAMELIB_AABB_COLLISON_HPP
#define GAMELIB_AABB_COLLISON_HPP

#include "gamelib/components/CollisionComponent.hpp"

namespace gamelib
{
    class AABB : public CollisionComponent
    {
        public:
            ASSIGN_NAMETAG("AABBCollision");

        public:
            AABB();
            AABB(float x, float y, float w, float h, unsigned int flags = 0);
            AABB(const math::AABBf& aabb, unsigned int flags = 0);
            virtual ~AABB() {}

            auto intersect(const math::Point2f& point) const -> bool final override;
            auto intersect(const math::Line2f& line) const   -> Intersection final override;
            auto intersect(const math::AABBf& rect) const    -> Intersection final override;

            auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection final override;

            auto setSize(const math::Vec2f& size) -> void;
            auto setSize(float w, float h)        -> void;

            auto getBBox() const     -> math::AABBf final override;

        protected:
            virtual auto _onChanged(const sf::Transform& old) -> void override;

        protected:
            math::Vec2f _size;
            math::AABBf _rect;
    };
}

#endif
