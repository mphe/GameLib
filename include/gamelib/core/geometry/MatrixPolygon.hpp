#ifndef GAMELIB_MATRIX_POLYGON_HPP
#define GAMELIB_MATRIX_POLYGON_HPP

#include "math/geometry/Polygon.hpp"
#include <SFML/Graphics/Transform.hpp>

namespace gamelib
{
    class MatrixPolygon : public math::BasePolygon<float>
    {
        public:
            MatrixPolygon(math::PolygonType type = math::TriangleStrip, math::NormalDirection ndir = math::NormalBoth);
            MatrixPolygon(size_t size, math::PolygonType type = math::TriangleStrip, math::NormalDirection ndir = math::NormalBoth);
            virtual ~MatrixPolygon() {};

            virtual auto add(const math::Point2f& point)        -> void;
            virtual auto edit(size_t i, const math::Point2f& p) -> void;
            virtual auto get(int i) const                       -> math::Point2f;

            auto setMatrix(const sf::Transform& mat) -> void;
            auto getMatrix() const -> const sf::Transform&;

        private:
            sf::Transform _matrix;
    };
}

#endif
