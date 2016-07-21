#include "gamelib/movement/TileWalker.hpp"

namespace gamelib
{
    TileWalker::TileWalker(float x, float y, float w, float h, const BaseTilemapCollider& c) :
        AABBCollision(x, y, w, h),
        _col(&c)
    { }

    void TileWalker::move(float x, float y)
    {
        _rect.pos.set(x, y);
    }

    void TileWalker::moveContact(float dx, float dy)
    {
        if (dx)
            _moveContact(dx, 0);
        if (dy)
            _moveContact(dy, 1);
    }

    geometry::Vector2<float> TileWalker::getPosition() const
    {
        return _rect.pos;
    }

    void TileWalker::_moveContact(float val, int index)
    {
        // Move first, then move outside
        _rect.pos[index] += val;

        // Move outside
        if (_col->intersects(_rect))
        {
            auto tsize = _col->getTileSize();

            // Calculate the amount of pixels the object has moved the current tile.
            float a;
            if (val < 0)
            {
                if (_rect.pos[index] < 0)
                    a = tsize[index] + std::fmod(_rect.pos[index], tsize[index]);
                else
                    a = tsize[index] - std::fmod(_rect.pos[index], tsize[index]);
            }
            else if (val > 0)
            {
                if (_rect.pos[index] < 0)
                    a = tsize[index] + std::fmod(_rect.pos[index] + _rect.size[index], tsize[index]);
                else
                    a = std::fmod(_rect.pos[index] + _rect.size[index], tsize[index]);
            }

            _rect.pos[index] -= math::sign(val) * a;

            while (_col->intersects(_rect))
                _rect.pos[index] -= math::sign(val) * tsize[index];
        }
    }
}
