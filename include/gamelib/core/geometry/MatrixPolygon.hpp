#ifndef GAMELIB_MATRIX_POLYGON_HPP
#define GAMELIB_MATRIX_POLYGON_HPP

#include <vector>
#include "math/geometry/Polygon.hpp"
#include <SFML/Graphics/Transform.hpp>

// TODO: cache transformed points

namespace gamelib
{
    class Polygon : public math::BasePolygon<float>
    {
        public:
            Polygon();
            Polygon(size_t capacity);
            virtual ~Polygon() {};

            virtual math::Point2f get(size_t i) const override;
            virtual size_t        size() const        override;

        protected:
            virtual void _add(const math::Point2f& point)          override;
            virtual void _edit(size_t i, const math::Point2f& p)   override;
            virtual void _insert(size_t i, const math::Point2f& p) override;
            virtual void _remove(size_t i)                         override;
            virtual void _clear()                                  override;

        private:
            std::vector<math::Point2f> _vertices;
    };

    class PolygonTransformer : public math::PolygonAdapter<float>
    {
        public:
            PolygonTransformer(math::AbstractPolygon<float>& pol);
            virtual ~PolygonTransformer() {};

            auto setMatrix(const sf::Transform& mat) -> void;
            auto getMatrix() const                   -> const sf::Transform&;

            virtual math::Point2f get(size_t i) const override;

        protected:
            virtual void _add(const math::Point2f& point)          override;
            virtual void _edit(size_t i, const math::Point2f& p)   override;
            virtual void _insert(size_t i, const math::Point2f& p) override;

            math::Point2f _getInverse(const math::Point2f& p) const;

        private:
            sf::Transform _matrix;
    };
}

#endif
