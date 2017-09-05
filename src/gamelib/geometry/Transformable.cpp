#include "gamelib/geometry/Transformable.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    Transformable::Transformable(unsigned int supported) :
        _supported(supported)
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
        LOG_DEBUG_WARN("Setting position is not supported by this object");
    }

    void Transformable::scale(const math::Vec2f& scale)
    {
        LOG_DEBUG_WARN("Scaling is not supported by this object");
    }

    void Transformable::rotate(float angle)
    {
        LOG_DEBUG_WARN("Rotating is not supported by this object");
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
        setScale(0, 0);
        setRotation(0);
    }
}
