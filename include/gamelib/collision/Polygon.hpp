#ifndef GAMELIB_POLYGON_COLLISON_HPP
#define GAMELIB_POLYGON_COLLISON_HPP

#include "Collidable.hpp"
#include "math/geometry/Polygon.hpp"

namespace gamelib
{
    class Polygon : public Collidable
    {
        public:
            Polygon();
            Polygon(math::PolygonType type, unsigned int flags = 0, void* owner = nullptr);

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const   -> Intersection;
            auto intersect(const math::AABBf& rect) const    -> Intersection;

            auto move(float x, float y)         -> void;
            auto setPosition(float x, float y)  -> void;

            auto getPosition() const    -> const math::Vec2f&;
            auto getBBox() const        -> const math::AABBf;

        public:
            math::Polygon<float> polygon;
    };
}

#endif
