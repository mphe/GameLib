#ifndef GAMELIB_TILEMAP_COLLIDER_INL
#define GAMELIB_TILEMAP_COLLIDER_INL

#include "TilemapCollider.hpp"
#include <limits>

namespace gamelib
{
    template <class T>
    TilemapCollider<T>::TilemapCollider(const StaticTilemap<T>& map, Callback cb) :
        _map(map),
        _callback(cb)
    { }

    template <class T>
    bool TilemapCollider<T>::contains(const geometry::Vector2<float>& point) const
    {
        const T* t = _map.get(point.x, point.y);
        if (t && internal::CollidableForwarder<T>::contains(*t, point))
            return _callback(*t);
        return false;
    }

    template <class T>
    bool TilemapCollider<T>::intersects(const geometry::AABB<float>& rect) const
    {
        const float w = rect.pos.x + rect.size.x,
                    h = rect.pos.y + rect.size.y;

        float y = rect.pos.y - (int)rect.pos.y % _map._tsize[1];
        if (rect.pos.y < 0)
            y -= _map._tsize[1];

        for (; y < h; y += _map._tsize[1])
        {
            float x = rect.pos.x - (int)rect.pos.x % _map._tsize[0];
            if (rect.pos.x < 0)
                x -= _map._tsize[0];

            for (; x < w; x += _map._tsize[0])
            {
                const T* t = _map.get(x, y);
                if (t && internal::CollidableForwarder<T>::intersects(*t, rect))
                    if (_callback(*t))
                        return true;
            }
        }
        return false;
    }

    template <class T>
    geometry::AABB<float> TilemapCollider<T>::getBBox() const
    {
        const float fmin = std::numeric_limits<float>::min(),
                    fmax = std::numeric_limits<float>::max();
        return geometry::AABB<float>(
            _map._virtsize[0] == infiniteRepeat ? fmin : 0,
            _map._virtsize[1] == infiniteRepeat ? fmin : 0,
            _map._virtsize[0] == infiniteRepeat ? fmax : _map._tsize[0] * _map._size[0],
            _map._virtsize[1] == infiniteRepeat ? fmax : _map._tsize[1] * _map._size[1]
            );
    }
}

#endif
