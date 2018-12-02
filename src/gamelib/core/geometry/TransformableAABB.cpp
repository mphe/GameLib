#include "gamelib/core/geometry/TransformableAABB.hpp"
#include "gamelib/utils/conversions.hpp"

namespace gamelib
{
	TransformableAABB::TransformableAABB() :
        TransformableAABB(0, 0, 0, 0)
	{ }

	TransformableAABB::TransformableAABB(float x, float y, float w, float h) :
        TransformableAABB(math::AABBf(x, y, w, h))
	{ }

	TransformableAABB::TransformableAABB(const math::AABBf& rect) :
        _size(rect.size),
        _rect(rect)
	{
        _setSupportedOps(true, true, false);
    }

	const math::AABBf& TransformableAABB::getBBox() const
	{
        return _rect;
	}

	const math::Vec2f& TransformableAABB::getSize() const
	{
        return _size;
	}

	void TransformableAABB::setSize(float w, float h)
	{
        setSize(math::Vec2f(w, h));
	}

	void TransformableAABB::setSize(const math::Vec2f& size)
	{
        _size = size;
        _rect.size = _size * getScale();
        _markDirty();
	}

	void TransformableAABB::_onChanged(const sf::Transform& old)
	{
        _rect.pos = convert(getMatrix().transformPoint(0, 0));
        _rect.size = _size * getScale();
	}
}
