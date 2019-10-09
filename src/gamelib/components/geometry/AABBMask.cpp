#include "gamelib/components/geometry/AABBMask.hpp"
#include "gamelib/properties/PropComponent.hpp"
#include "math/geometry/intersect.hpp"

namespace gamelib
{
    AABBMask::AABBMask(unsigned int flags_) :
        _comp(nullptr)
    {
        flags = flags_;
        _setSupportedOps(false, false, false);
        registerProperty(_props, "component", _comp, *this, PROP_METHOD(_comp, setComponent));
    }


    math::AABBf AABBMask::getBBox() const
    {
        return _comp->getTransform()->getBBox();
    }

    bool AABBMask::intersect(const math::Point2f& point) const
    {
        return math::intersect(getBBox(), point);
    }

    Intersection AABBMask::intersect(const math::Line2f& line) const
    {
        return math::intersect(line, getBBox());
    }

    Intersection AABBMask::intersect(const math::AABBf& rect) const
    {
        return math::intersect(getBBox(), rect);
    }

    Intersection AABBMask::sweep(const math::AABBf& rect, const math::Vec2f& vel) const
    {
        return math::sweep(rect, vel, getBBox());
    }

    bool AABBMask::setComponent(BaseCompRef c)
    {
        if (static_cast<Component*>(this) == c.get())
        {
            LOG_ERROR("Can't assign self");
            return false;
        }
        else if (c->getTransform())
        {
            _comp = c;
            return true;
        }
        else
        {
            LOG_ERROR("Component is not a Transformable");
            return false;
        }
    }

    BaseCompRef AABBMask::getComponent() const
    {
        return _comp;
    }
}
