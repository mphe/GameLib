#include "gamelib/core/geometry/Transformable.hpp"
#include "gamelib/core/geometry/GroupTransform.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/utils/conversions.hpp"

// TODO:

namespace gamelib
{
    // Prevent division by zero errors
    math::Vec2f assureNonZero(const math::Vec2f& vec)
    {
        return math::Vec2f(
                math::almostZero(vec.x) ? zeroscale * math::sign(vec.x) : vec.x,
                math::almostZero(vec.y) ? zeroscale * math::sign(vec.y) : vec.y
            );
    }

    TransformData::TransformData() : scale(1, 1), angle(0)
    { }

    sf::Transform TransformData::getMatrix() const
    {
        if (origin.asVector().isZero() && pos.asVector().isZero()
                && scale == math::Vec2f(1) && angle == 0.f)
            return sf::Transform::Identity;

        sf::Transform m;
        m.translate(pos.x - origin.x, pos.y - origin.y);
        m.rotate(angle, origin.x, origin.y);
        m.scale(scale.x, scale.y, origin.x, origin.y);
        return m;
    }

    void TransformData::reset()
    {
        origin.set(0);
        pos.set(0);
        scale.fill(1);
        angle = 0;
    }

    TransformData& TransformData::operator-=(const TransformData& rhs)
    {
        pos -= rhs.pos.asVector();
        scale /= assureNonZero(rhs.scale);
        angle -= rhs.angle;
        return *this;
    }

    TransformData& TransformData::operator+=(const TransformData& rhs)
    {
        pos += rhs.pos.asVector();
        scale *= rhs.scale;
        angle += rhs.angle;
        return *this;
    }





    Transformable::Transformable() :
        Transformable(true, true, true)
    { }

    Transformable::Transformable(bool movable, bool scalable, bool rotatable) :
        _movable(movable),
        _scalable(scalable),
        _rotatable(rotatable),
        _parent(nullptr)
    { }


    // --- set local ---
    void Transformable::setLocalPosition(const math::Point2f& pos)
    {
        if (pos == _local.pos || !_movable)
            return;
        _local.pos = pos;
        _updateMatrix();
    }

    void Transformable::setLocalScale(const math::Vec2f& scale)
    {
        if (scale == _local.scale || !_scalable)
            return;

        _local.scale = assureNonZero(scale);
        _updateMatrix();
    }

    void Transformable::setLocalRotation(float angle)
    {
        if (angle == _local.angle || !_rotatable)
            return;
        _local.angle = angle;
        _updateMatrix();
    }

    void Transformable::setLocalTransformation(const TransformData& data)
    {
        if (data.origin == _local.origin && data.pos == _local.pos
                && data.scale == _local.scale && data.angle == _local.angle)
            return;

        _local = data;
        _updateMatrix();
    }


    // --- get local ---
    const math::Point2f& Transformable::getLocalPosition() const
    {
        return _local.pos;
    }

    const math::Vec2f& Transformable::getLocalScale() const
    {
        return _local.scale;
    }

    float Transformable::getLocalRotation() const
    {
        return _local.angle;
    }

    const TransformData& Transformable::getLocalTransformation() const
    {
        return _local;
    }

    sf::Transform Transformable::getLocalMatrix() const
    {
        return _local.getMatrix();
    }

    math::AABBf Transformable::getLocalBBox() const
    {
        return convert(getMatrix().getInverse().transformRect(convert(getBBox())));
    }


    // --- set global ---
    void Transformable::setPosition(float x, float y)
    {
        setPosition(math::Point2f(x, y));
    }

    void Transformable::setPosition(const math::Point2f& pos)
    {
        move(pos - getPosition());
    }

    void Transformable::setScale(float x, float y)
    {
        setScale(math::Vec2f(x, y));
    }

    void Transformable::setScale(const math::Vec2f& scale_)
    {
        scale(scale_ / getScale());
    }

    void Transformable::setRotation(float angle)
    {
        rotate(angle - getRotation());
    }

    void Transformable::setTransformation(const TransformData& data)
    {
        if (data.pos == _global.pos && data.scale == _global.scale && data.angle == _global.angle)
            return;

        auto diff = data;
        diff -= getTransformation();
        _local += diff;
        _updateMatrix();
    }


    // --- get global ---
    const math::Point2f& Transformable::getPosition() const
    {
        return _global.pos;
    }

    const math::Vec2f& Transformable::getScale() const
    {
        return _global.scale;
    }

    float Transformable::getRotation() const
    {
        return _global.angle;
    }

    const sf::Transform& Transformable::getMatrix() const
    {
        return _matrix;
    }

    TransformData Transformable::getTransformation() const
    {
        TransformData data;
        data.pos = _global.pos;
        data.scale = _global.scale;
        data.angle = _global.angle;
        return data;
    }


    // --- relative ---
    void Transformable::move(float x, float y)
    {
        move(math::Vec2f(x, y));
    }

    void Transformable::move(const math::Vec2f& rel)
    {
        setLocalPosition(getLocalPosition() + rel);
    }

    void Transformable::scale(float x, float y)
    {
        scale(math::Vec2f(x, y));
    }

    void Transformable::scale(const math::Vec2f& scale)
    {
        setLocalScale(getLocalScale() * scale);
    }

    void Transformable::rotate(float angle)
    {
        setLocalRotation(getLocalRotation() + angle);
    }


    // --- origin ---
    void Transformable::setOrigin(const math::Point2f& origin)
    {
        if (origin == _local.origin)
            return;
        _local.origin = origin;
        _updateMatrix();
    }

    void Transformable::setOrigin(float x, float y)
    {
        setOrigin(math::Point2f(x, y));
    }

    const math::Point2f& Transformable::getOrigin() const
    {
        return _local.origin;
    }


    // --- misc ---
    bool Transformable::isMovable() const
    {
        return _movable;
    }

    bool Transformable::isScalable() const
    {
        return _scalable;
    }

    bool Transformable::isRotatable() const
    {
        return _rotatable;
    }

    void Transformable::reset()
    {
        _local.reset();
        _updateMatrix();
    }

    GroupTransform* Transformable::getParent() const
    {
        return _parent;
    }


    // --- internal ---
    void Transformable::_setParent(GroupTransform* parent)
    {
        if (parent == _parent)
            return;

        _parent = parent;
        _updateMatrix();
    }

    void Transformable::_updateMatrix()
    {
        auto old = _matrix;

        if (_parent)
        {
            if (!_movable)
                _local.pos.asVector() = -_parent->getPosition().asVector();
            if (!_scalable)
                _local.scale = math::Vec2f(1) / _parent->getScale();
            if (!_rotatable)
                _local.angle = -_parent->getRotation();

            _matrix = _parent->getMatrix();
            _matrix *= _local.getMatrix();
            _global.pos = _parent->getPosition() + _local.pos.asVector();
            _global.scale = _parent->getScale() * _local.scale;
            _global.angle = _parent->getRotation() + _local.angle;
        }

        if (!_movable)
            _local.pos.set(0);
        if (!_scalable)
            _local.scale.fill(1);
        if (!_rotatable)
            _local.angle = 0.f;

        if (!_parent)
        {
            _matrix = _local.getMatrix();
            _global.pos = _local.pos;
            _global.scale = _local.scale;
            _global.angle = _local.angle;
        }

        _onChanged(old);
        _markDirty();
    }

    void Transformable::_setSupportedOps(bool movable, bool scalable, bool rotatable)
    {
        if (movable == _movable && scalable == _scalable && rotatable == _rotatable)
            return;
        _movable = movable;
        _scalable = scalable;
        _rotatable = rotatable;
        _updateMatrix();
    }

    void Transformable::_markDirty() const
    {
        if (_parent)
            _parent->_dirty = true;
    }


    // --- operators ---
    Transformable& Transformable::operator-=(const TransformData& rhs)
    {
        _local -= rhs;
        _updateMatrix();
        return *this;
    }

    Transformable& Transformable::operator+=(const TransformData& rhs)
    {
        _local += rhs;
        _updateMatrix();
        return *this;
    }
}
