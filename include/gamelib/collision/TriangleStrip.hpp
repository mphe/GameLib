#ifndef GAMELIB_TRIANGLESTRIP_COLLISON_HPP
#define GAMELIB_TRIANGLESTRIP_COLLISON_HPP

#include "Collidable.hpp"
#include "math/geometry/TriangleStrip.hpp"

namespace gamelib
{
    class TriangleStrip : public Collidable
    {
        public:
            TriangleStrip(unsigned int flags = 0, void* owner = nullptr);

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const   -> Intersection;
            auto intersect(const math::AABBf& rect) const    -> Intersection;

            auto move(float x, float y)         -> void;
            auto setPosition(float x, float y)  -> void;

            auto getPosition() const    -> const math::Vec2f&;
            auto getBBox() const        -> const math::AABBf;

        public:
            math::TriangleStrip<float> polygon;
    };
}

#endif
