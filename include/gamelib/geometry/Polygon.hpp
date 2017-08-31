#ifndef GAMELIB_POLYGON_COLLISON_HPP
#define GAMELIB_POLYGON_COLLISON_HPP

#include "gamelib/ecs/CollisionComponent.hpp"
#include "math/geometry/TriangleStrip.hpp"
#include "math/geometry/LineStrip.hpp"

namespace gamelib
{
    namespace detail
    {
        template <typename T>
        class Polygon : public CollisionComponent
        {
            public:
                Polygon(unsigned int flags_ = 0) :
                    _scale(1, 1)
                {
                    flags = flags_;
                    _supported = movable | scalable; // TODO: rotation
                }

                virtual ~Polygon() {}

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

                auto move(const math::Vec2f& rel) -> void
                {
                    polygon.move(rel.x, rel.y);
                }

                auto scale(const math::Vec2f& scale) -> void
                {
                    for (size_t i = 0; i < polygon.size(); ++i)
                    {
                        auto p = polygon.getRaw(i).asVector() * scale;
                        polygon.edit(i, polygon.getOffset().asPoint() + p);
                    }
                    _scale *= scale;
                }

                auto getPosition() const -> const math::Point2f&
                {
                    return polygon.getOffset().asPoint();
                }

                auto getScale() const -> const math::Vec2f&
                {
                    return _scale;
                }

                auto getBBox() const -> const math::AABBf&
                {
                    return polygon.getBBox();
                }

            public:
                math::Vec2f _scale;
                T polygon;
        };
    }

    typedef detail::Polygon<math::TriangleStrip<float>> TriangleStripComponent;
    typedef detail::Polygon<math::LineStrip<float>> LineStripComponent;

    // Deprecated, only for backwards compatibility
    typedef detail::Polygon<math::TriangleStrip<float>> TriangleStrip;
    typedef detail::Polygon<math::LineStrip<float>> LineStrip;
}

#endif
