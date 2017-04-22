#ifndef GAMELIB_POLYGON_COLLISON_HPP
#define GAMELIB_POLYGON_COLLISON_HPP

#include "Collidable.hpp"
#include "math/geometry/Polygon.hpp"
#include "math/geometry/TriangleStrip.hpp"
#include "math/geometry/LineStrip.hpp"

namespace gamelib
{
    namespace detail
    {
        template <typename T>
        class Polygon : public Collidable
        {
            public:
                Polygon(unsigned int flags = 0, void* owner = nullptr) :
                    Collidable(flags, owner)
                {}

                ~Polygon() {}

                auto intersect(const math::Point2f& point) const -> bool
                {
                    return polygon.intersect(point);
                }

                auto intersect(const math::Line2f& line) const -> Intersection
                {
                    return polygon.intersect(line);
                }

                auto intersect(const math::AABBf& rect) const -> Intersection
                {
                    throw "Not implemented";
                }

                auto move(float x, float y) -> void
                {
                    polygon.move(x, y);
                }

                auto setPosition(float x, float y) -> void
                {
                    polygon.setOffset(x, y);
                }

                auto getPosition() const -> const math::Vec2f&
                {
                    return polygon.getOffset();
                }

                auto getBBox() const -> const math::AABBf
                {
                    return polygon.getBBox();
                }

            public:
                T polygon;
        };
    }

    typedef detail::Polygon<math::TriangleStrip<float>> TriangleStrip;
    typedef detail::Polygon<math::LineStrip<float>> LineStrip;
}

#endif
