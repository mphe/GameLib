#ifndef GAMELIB_AABB_COLLISON_HPP
#define GAMELIB_AABB_COLLISON_HPP

#include "gamelib/components/CollisionComponent.hpp"

namespace gamelib
{
    class AABB : public CollisionComponent
    {
        public:
            constexpr static const char* name = "AABBCollision";

        public:
            AABB();
            AABB(float x, float y, float w, float h, unsigned int flags = 0);
            AABB(const math::AABBf& aabb, unsigned int flags = 0);
            virtual ~AABB() {}

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const   -> Intersection;
            auto intersect(const math::AABBf& rect) const    -> Intersection;

            auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection;

            auto setSize(const math::Vec2f& size) -> void;
            auto setSize(float w, float h)        -> void;

            auto move(const math::Vec2f& rel)    -> void;
            auto scale(const math::Vec2f& scale) -> void;
            auto getPosition() const             -> const math::Point2f&;
            auto getScale() const                -> const math::Vec2f&;

            auto getBBox() const -> const math::AABBf&;

            virtual auto writeToJson(Json::Value& node) -> void;

        protected:
            math::Vec2f _scale;
            math::AABBf _rect;
    };
}

#endif
