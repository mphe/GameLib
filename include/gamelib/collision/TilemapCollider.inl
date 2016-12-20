#ifndef GAMELIB_TILEMAP_COLLIDER_INL
#define GAMELIB_TILEMAP_COLLIDER_INL

#include "TilemapCollider.hpp"
#include "gamelib/utils/log.hpp"
#include <limits>

namespace gamelib
{
    template <class T>
    TilemapCollider<T>::TilemapCollider(const StaticTilemap<T>& map) :
        _map(&map),
        _callback([](const T&){ return true; })
    { }

    template <class T>
    template <class F>
    TilemapCollider<T>::TilemapCollider(const StaticTilemap<T>& map, F cb) :
        _map(&map),
        _callback(cb)
    { }

    template <class T>
    template <class F>
    void TilemapCollider<T>::setCallback(F callback)
    {
        _callback = callback;
    }

    template <class T>
    bool TilemapCollider<T>::intersect(const math::Point2f& point) const
    {
        const T* t = _map->get(point.x, point.y);
        if (t && internal::CollidableForwarder<T>::contains(*t, point - _pos))
            return _callback(*t);
        return false;
    }

    template <class T>
    Intersection TilemapCollider<T>::intersect(const math::AABBf& rect_) const
    {
        math::AABBf rect(rect_.pos - _pos, rect_.size);
        const float w = rect.pos.x + rect.size.x,
                    h = rect.pos.y + rect.size.y;

        float y = rect.pos.y - (int)rect.pos.y % _map->_tsize[1];
        if (rect.pos.y < 0)
            y -= _map->_tsize[1];

        for (; y < h; y += _map->_tsize[1])
        {
            float x = rect.pos.x - (int)rect.pos.x % _map->_tsize[0];
            if (rect.pos.x < 0)
                x -= _map->_tsize[0];

            for (; x < w; x += _map->_tsize[0])
            {
                const T* t = _map->get(x, y);
                if (t && internal::CollidableForwarder<T>::intersects(*t, rect))
                    if (_callback(*t))
                    {
                        // TODO: Do this properly
                        LOG_DEBUG_WARN("Tilemap vs AABB intersection check not properly implemented! Don't use delta!");
                        return Intersection(math::Vec2f());
                    }
            }
        }
        return Intersection();
    }

    template <class T>
    Intersection TilemapCollider<T>::intersect(const math::Line2f& line) const
    {
        throw "Not implemented";
    }

    template <class T>
    void TilemapCollider<T>::move(float x, float y)
    {
        _pos += math::Vec2f(x, y);
    }

    template <class T>
    void TilemapCollider<T>::setPosition(float x, float y)
    {
        _pos.x = x;
        _pos.y = y;
    }

    template <class T>
    const math::Vec2f& TilemapCollider<T>::getPosition() const
    {
        return _pos;
    }

    template <class T>
    const math::AABBf TilemapCollider<T>::getBBox() const
    {
        const float fmin = std::numeric_limits<float>::min(),
                    fmax = std::numeric_limits<float>::max();

        // TODO: Seems wrong: not checking for fixed virtual size

        return math::AABB<float>(
            _map->_virtsize[0] == infiniteRepeat ? fmin : _pos.x,
            _map->_virtsize[1] == infiniteRepeat ? fmin : _pos.y,
            _map->_virtsize[0] == infiniteRepeat ? fmax : _map->_tsize[0] * _map->_size[0],
            _map->_virtsize[1] == infiniteRepeat ? fmax : _map->_tsize[1] * _map->_size[1]
            );
    }

    template <class T>
    const int* TilemapCollider<T>::getTileSize() const
    {
        return _map->getTileSize();
    }
}

#endif
