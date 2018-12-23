#ifndef GAMELIB_AABB_MASK_HPP
#define GAMELIB_AABB_MASK_HPP

#include "gamelib/components/CollisionComponent.hpp"

namespace gamelib
{
    class AABBMask : public CollisionComponent
    {
        public:
            constexpr static const char* name = "AABBMask";

        public:
            AABBMask(unsigned int flags = 0);
            ~AABBMask() {}

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const   -> Intersection;
            auto intersect(const math::AABBf& rect) const    -> Intersection;

            auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection;

            auto getBBox() const -> const math::AABBf&;

            auto setComponent(const Component* c) -> bool;
            auto getComponent() const             -> const Component*;

        protected:
            auto _onChanged(const sf::Transform&) -> void {};

        private:
            const Component* _comp;
    };
}

#endif
