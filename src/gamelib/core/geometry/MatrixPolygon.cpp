#include "gamelib/core/geometry/MatrixPolygon.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
    MatrixPolygon::MatrixPolygon(math::PolygonType type, math::NormalDirection ndir) :
        BasePolygon(type, ndir)
    { }

    MatrixPolygon::MatrixPolygon(size_t size, math::PolygonType type, math::NormalDirection ndir) :
        BasePolygon(size, type, ndir)
    { }

    void MatrixPolygon::add(const math::Point2f& p)
    {
        addRaw(convert(_matrix.getInverse().transformPoint(convert(p))).asPoint());
    }

    void MatrixPolygon::edit(size_t i, const math::Point2f& p)
    {
        editRaw(i, convert(_matrix.getInverse().transformPoint(convert(p))).asPoint());
    }

    math::Point2f MatrixPolygon::get(int i) const
    {
        return convert(_matrix.transformPoint(convert(getRaw(i)))).asPoint();
    }


    void MatrixPolygon::setMatrix(const sf::Transform& mat)
    {
        _matrix = mat;
        _recalculate();
    }

    const sf::Transform& MatrixPolygon::getMatrix() const
    {
        return _matrix;
    }
}
