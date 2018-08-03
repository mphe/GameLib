#include "gamelib/core/geometry/Transformable.hpp"
#include "gamelib/core/geometry/GroupTransform.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Transformable::Transformable(unsigned int supported) :
        _supported(supported),
        _parent(nullptr)
    { }

    void Transformable::move(float x, float y)
    {
        move(math::Vec2f(x, y));
    }

    void Transformable::setPosition(float x, float y)
    {
        setPosition(math::Point2f(x, y));
    }

    void Transformable::scale(float x, float y)
    {
        scale(math::Vec2f(x, y));
    }

    void Transformable::setScale(float x, float y)
    {
        setScale(math::Vec2f(x, y));
    }

    void Transformable::setPosition(const math::Point2f& pos)
    {
        move(pos - getPosition());
    }

    void Transformable::setScale(const math::Vec2f& scale_)
    {
        math::Vec2f fixed(scale_);
        if (fixed.x == 0)
            fixed.x = 0.00001;
        if (fixed.y == 0)
            fixed.y = 0.00001;
        scale(fixed / getScale());
    }

    void Transformable::setRotation(float angle)
    {
        rotate(angle - getRotation());
    }


    void Transformable::move(const math::Vec2f& rel)
    {
        markDirty();
    }

    void Transformable::scale(const math::Vec2f& scale)
    {
        markDirty();
    }

    void Transformable::rotate(float angle)
    {
        markDirty();
    }

    const math::AABBf& Transformable::getBBox() const
    {
        static const math::AABBf def;
        LOG_DEBUG_WARN("Bounding Box is not supported by this object");
        return def;
    }


    const math::Point2f& Transformable::getPosition() const
    {
        static const math::Point2f def;
        return def;
    }

    const math::Vec2f& Transformable::getScale() const
    {
        static const math::Vec2f def(1, 1);
        return def;
    }

    float Transformable::getRotation() const
    {
        return 0;
    }

    unsigned int Transformable::getSupportedOps() const
    {
        return _supported;
    }

    void Transformable::reset()
    {
        setPosition(0, 0);
        setScale(1, 1);
        setRotation(0);
    }

    GroupTransform* Transformable::getParent() const
    {
        return _parent;
    }

    void Transformable::markDirty() const
    {
        if (_parent)
            _parent->_dirty = true;
    }


    Transformable& Transformable::operator-=(const Transformable& rhs)
    {
        move(-rhs.getPosition().asVector());
        scale(math::Vec2f(1, 1) / rhs.getScale());
        rotate(-rhs.getRotation());
        markDirty();
        return *this;
    }

    Transformable& Transformable::operator+=(const Transformable& rhs)
    {
        move(rhs.getPosition().asVector());
        scale(rhs.getScale());
        rotate(rhs.getRotation());
        markDirty();
        return *this;
    }
}
