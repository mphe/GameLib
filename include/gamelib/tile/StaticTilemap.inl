#ifndef GAMELIB_STATIC_TILEMAP_INL
#define GAMELIB_STATIC_TILEMAP_INL

#include "StaticTilemap.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    constexpr int X = 0;
    constexpr int Y = 1;

    template <class T>
    StaticTilemap<T>::StaticTilemap() :
        _size{0},
        _virtsize{0},
        _tsize{0}
    { }

    template <class T>
    void StaticTilemap<T>::create(int w, int h)
    {
        _size[X] = w;
        _size[Y] = h;
        _map.resize(w * h, InvalidTile);
    }

    template <class T>
    void StaticTilemap<T>::create(int w, int h, const std::vector<TileID>& map)
    {
        _size[X] = w;
        _size[Y] = h;
        _map = map;
        if (map.size() < w * h)
            _map.resize(w * h, InvalidTile);
    }

    template <class T>
    void StaticTilemap<T>::create(int w, int h, std::vector<TileID>&& map)
    {
        _size[X] = w;
        _size[Y] = h;
        // map must be resized, not _map, because map is move-assigned to _map.
        if (map.size() < w * h)
            map.resize(w * h, InvalidTile);
        _map = map;
    }

    template <class T>
    void StaticTilemap<T>::destroy()
    {
        _map.clear();
        std::vector<TileID>().swap(_map);
        _tiles.clear();
        std::vector<T>().swap(_tiles);
        for (int i = 0; i < 2; ++i)
            _size[i] = _virtsize[i] = _tsize[i] = 0;
        LOG_DEBUG_WARN("TileMap destroyed");
    }

    template <class T>
    const T* StaticTilemap<T>::get(int x, int y) const
    {
        return getTile(x / _tsize[X] - (x < 0 ? 1 : 0),
                       y / _tsize[Y] - (y < 0 ? 1 : 0));
    }

    template <class T>
    const T* StaticTilemap<T>::getTile(int x, int y) const
    {
        const TileID* t = _get(x, y);
        return !t || *t == InvalidTile ? NULL : &_tiles[*t];
    }

    template <class T>
    void StaticTilemap<T>::setVirtualSize(int w, int h)
    {
        _virtsize[X] = w == noRepeat ? _size[X] : w;
        _virtsize[Y] = h == noRepeat ? _size[Y] : h;
    }

    template <class T>
    void StaticTilemap<T>::setTileSize(int w, int h)
    {
        _tsize[X] = w;
        _tsize[Y] = h;
    }

    template <class T>
    void StaticTilemap<T>::setTileSet(const std::vector<T>& set)
    {
        if (set.size() < _tiles.size())
            LOG_WARN("New tileset has less elements than the old one.");
        _tiles = set;
    }

    template <class T>
    void StaticTilemap<T>::setTileSet(std::vector<T>&& set)
    {
        if (set.size() < _tiles.size())
            LOG_WARN("New tileset has less elements than the old one.");
        _tiles = set;
    }

    template <class T>
    void StaticTilemap<T>::globalToTileCoords(int* x, int* y) const
    {
        *x = *x / _tsize[X] - (*x < 0 ? 1 : 0);
        *y = *y / _tsize[Y] - (*y < 0 ? 1 : 0);
    }

    template <class T>
    void StaticTilemap<T>::tileToGlobalCoords(int* x, int* y) const
    {
        *x = _tsize[X] * *x;
        *y = _tsize[Y] * *y;
    }


    template <class T>
    void StaticTilemap<T>::set(int x, int y, TileID id)
    {
        (*_get(x, y)) = id;
    }

    template <class T>
    TileID* StaticTilemap<T>::_get(int x, int y)
    {
        return const_cast<TileID*>(static_cast<const StaticTilemap<T>*>(this)->_get(x, y));
    }

    template <class T>
    const TileID* StaticTilemap<T>::_get(int x, int y) const
    {
        x = _adaptCoords(x, X);
        y = _adaptCoords(y, Y);
        if (x < _size[X] && y < _size[Y])
            return &_map[y * _size[X] + x];
        return NULL;
    }

    template <class T>
    int StaticTilemap<T>::_adaptCoords(int val, int index) const
    {
        if (_virtsize[index] == infiniteRepeat || val < _virtsize[index])
            return std::abs(val) % _size[index];
        return val;
    }
}

#endif
