#ifndef GAMELIB_POLYGON_COLLISON_HPP
#define GAMELIB_POLYGON_COLLISON_HPP

#include "gamelib/components/CollisionComponent.hpp"
#include "gamelib/core/geometry/MatrixPolygon.hpp"

namespace gamelib
{
    class PolygonCollider : public CollisionComponent
    {
        public:
            ASSIGN_NAMETAG("PolygonCollision");

        public:
            PolygonCollider(unsigned int flags = 0);
            virtual ~PolygonCollider() {}

            auto intersect(const math::Point2f& point) const -> bool final override;
            auto intersect(const math::Line2f& line) const   -> Intersection final override;
            auto intersect(const math::AABBf& rect) const    -> Intersection final override;

            auto sweep(const math::AABBf& rect, const math::Vec2f& vel) const -> Intersection final override;

            auto getBBox() const -> math::AABBf final override;

            auto loadFromJson(const Json::Value& node) -> bool final override;
            auto writeToJson(Json::Value& node) const  -> void final override;

            auto add(const math::Point2f& point, bool raw = true) -> void;
            auto edit(size_t i, const math::Point2f& p, bool raw = true) -> void;
            auto clear()            -> void;
            auto size() const       -> size_t;

            auto getLocal() const -> const math::AbstractPolygon<float>&;
            auto getGlobal() const -> const math::AbstractPolygon<float>&;

        protected:
            virtual auto _onChanged(const sf::Transform& old) -> void override;

        protected:
            Polygon _local;
            PolygonTransformer _global;
            math::FillType _filltype; // needed for properties
            math::NormalDirection _normaldir; // needed for properties
    };
}

#endif
