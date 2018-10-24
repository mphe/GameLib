#ifndef GAMELIB_POLYGON_COLLISON_HPP
#define GAMELIB_POLYGON_COLLISON_HPP

#include "gamelib/components/CollisionComponent.hpp"
#include "gamelib/core/geometry/MatrixPolygon.hpp"

namespace gamelib
{
    class Polygon : public CollisionComponent
    {
        public:
            constexpr static const char* name = "PolygonCollision";

        public:
            Polygon(unsigned int flags = 0);
            Polygon(math::PolygonType type, unsigned int flags = 0);
            virtual ~Polygon() {}

            auto intersect(const math::Point2f& point) const -> bool;
            auto intersect(const math::Line2f& line) const   -> Intersection;
            auto intersect(const math::AABBf& rect) const    -> Intersection;

            auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection;

            auto getBBox() const -> const math::AABBf&;

            auto loadFromJson(const Json::Value& node) -> bool;
            auto writeToJson(Json::Value& node)        -> void;

            auto add(const math::Point2f& point)        -> void;
            auto edit(size_t i, const math::Point2f& p) -> void;
            auto clear()            -> void;
            auto size() const       -> size_t;
            auto getPolygon() const -> const math::BasePolygon<float>&;

        protected:
            virtual auto _onChanged(const sf::Transform& old) -> void;

        protected:
            MatrixPolygon _polygon;
    };
}

#endif
