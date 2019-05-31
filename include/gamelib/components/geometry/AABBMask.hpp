#ifndef GAMELIB_AABB_MASK_HPP
#define GAMELIB_AABB_MASK_HPP

#include "gamelib/components/CollisionComponent.hpp"

namespace gamelib
{
    class AABBMask : public CollisionComponent
    {
        public:
            ASSIGN_NAMETAG("AABBMask");

        public:
            AABBMask(unsigned int flags = 0);
            ~AABBMask() {}

            auto intersect(const math::Point2f& point) const -> bool final override;
            auto intersect(const math::Line2f& line) const   -> Intersection final override;
            auto intersect(const math::AABBf& rect) const    -> Intersection final override;

            auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection final override;

            auto getBBox() const -> math::AABBf final override;

            auto setComponent(const Component* c) -> bool;
            auto getComponent() const             -> const Component*;

        protected:
            auto _onChanged(const sf::Transform&) -> void final override {};

        private:
            const Component* _comp;
    };
}

#endif
