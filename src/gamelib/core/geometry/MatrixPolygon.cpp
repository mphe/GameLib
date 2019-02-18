#include "gamelib/core/geometry/MatrixPolygon.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
    Polygon::Polygon() :
        BasePolygon(math::Filled, math::NormalLeft)
    { }

    Polygon::Polygon(size_t capacity) :
        Polygon()
    {
        _vertices.reserve(capacity);
    }

    math::Point2f Polygon::get(size_t i) const
    {
        return _vertices[i];
    }

    size_t Polygon::size() const
    {
        return _vertices.size();
    }

    void Polygon::_add(const math::Point2f& point)
    {
        _vertices.push_back(point);
    }

    void Polygon::_edit(size_t i, const math::Point2f& p)
    {
        _vertices[i] = p;
    }

    void Polygon::_insert(size_t i, const math::Point2f& p)
    {
        _vertices.insert(_vertices.begin() + i, p);
    }

    void Polygon::_remove(size_t i)
    {
        _vertices.erase(_vertices.begin() + i);
    }

    void Polygon::_clear()
    {
        _vertices.clear();
    }



    // PolygonTransformer
    PolygonTransformer::PolygonTransformer(math::AbstractPolygon<float>& pol)
        : PolygonAdapter(pol)
    { }

    void PolygonTransformer::setMatrix(const sf::Transform& mat)
    {
        _matrix = mat;
    }

    const sf::Transform& PolygonTransformer::getMatrix() const
    {
        return _matrix;
    }

    math::Point2f PolygonTransformer::get(size_t i) const
    {
        return convert(_matrix.transformPoint(convert(_pol->get(i)))).asPoint();
    }

    void PolygonTransformer::_add(const math::Point2f& p)
    {
        _pol->add(_getInverse(p));
    }

    void PolygonTransformer::_edit(size_t i, const math::Point2f& p)
    {
        _pol->edit(i, _getInverse(p));
    }

    void PolygonTransformer::_insert(size_t i, const math::Point2f& p)
    {
        _pol->insert(i, _getInverse(p));
    }

    math::Point2f PolygonTransformer::_getInverse(const math::Point2f& p) const
    {
        return convert(_matrix.getInverse().transformPoint(convert(p))).asPoint();
    }
}
