#ifndef GAMELIB_BASE_STATIC_TILEMAP_INL
#define GAMELIB_BASE_STATIC_TILEMAP_INL

#include "BaseStaticTileMap.hpp"
#include "gamelib/utils/log.hpp"

namespace gamelib
{
    constexpr int X = 0;
    constexpr int Y = 1;

    template <class T, class DataSetType>
    BaseStaticTileMap<T, DataSetType>::BaseStaticTileMap() :
        _size{0},
        _virtsize{0},
        _tsize{0},
        _dataset(NULL)
    { }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::create(int w, int h)
    {
        _size[X] = w;
        _size[Y] = h;
        _map.resize(w * h, InvalidTile);
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::create(int w, int h, const std::vector<TileID>& map)
    {
        _size[X] = w;
        _size[Y] = h;
        _map = map;
        if (map.size() < w * h)
            _map.resize(w * h, InvalidTile);
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::create(int w, int h, std::vector<TileID>&& map)
    {
        _size[X] = w;
        _size[Y] = h;
        // map must be resized, not _map, because map is move-assigned to _map.
        if (map.size() < w * h)
            map.resize(w * h, InvalidTile);
        _map = map;
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::destroy()
    {
        _map.clear();
        std::vector<TileID>().swap(_map);
        _tiles.clear();
        std::vector<T>().swap(_tiles);
        _dataset = NULL;
        for (int i = 0; i < 2; ++i)
            _size[i] = _virtsize[i] = _tsize[i] = 0;
        LOG_DEBUG_WARN("TileMap destroyed");
    }

    template <class T, class DataSetType>
    bool BaseStaticTileMap<T, DataSetType>::loadFromJson(const Json::Value& node)
    {
        //meta data
        create(node.get("width", 0).asInt(),
                node.get("height", 0).asInt());

        setVirtualSize(node.get("virtwidth", noRepeat).asInt(),
                node.get("virtheight", noRepeat).asInt());

        setTileSize(node.get("tilew", 0).asInt(),
                node.get("tileh", 0).asInt());

        LOG_DEBUG(LOG_DUMP(_virtsize[X]), ", ", LOG_DUMP(_virtsize[Y]));
        LOG_DEBUG(LOG_DUMP(_size[X]), ", ", LOG_DUMP(_size[Y]));
        LOG_DEBUG(LOG_DUMP(_tsize[X]), ", ", LOG_DUMP(_tsize[Y]));

        if (!_size[X] || !_size[Y] || !_tsize[X] || !_tsize[Y])
        {
            LOG_ERROR("Map size or tile size is 0.");
            return false;
        }

        TranslationMap trans;
        if (_dataset && (!node.isMember("objects") || node["objects"].size() == 0))
            // Import all -> import from current set again
            trans = importDataSet(*_dataset);
        else if (_dataset)
            trans = _loadTileSet(node);
        else
        {
            LOG_ERROR("Trying to load tileset from DataSet without DataSet.");
            return false;
        }

        LOG_DEBUG("Tileset contains ", trans.size(), " elements:");

        // Extend the translation map by the defined aliases
        if (node.isMember("alias"))
        {
            const auto& aliases = node["alias"];
            for (auto i = aliases.begin(); i != aliases.end(); ++i)
            {
                auto s = i.key().asString();
                if (i->isNumeric())
                    trans[s] = i->asInt();
                else
                {
                    auto it = trans.find(i->asString());
                    if (it != trans.end())
                        trans[s] = it->second;
                    else
                        LOG_WARN("Alias '", s, "' links to unknown value '", it->second, "' -> Skipping");
                }
            }
        }

        for (auto& i : trans)
            LOG_DEBUG(i.first, "\t->\t", i.second);

        if (node.isMember("map"))
        {
            const auto& tiles = node["map"];
            for (Json::ArrayIndex i = 0; i < _map.size() && i < tiles.size(); ++i)
            {
                TileID id = InvalidTile;
                if (tiles[i].isNumeric())
                    id = tiles[i].asInt();
                else if (trans.find(tiles[i].asString()) != trans.end())
                    id = trans.find(tiles[i].asString())->second;

                if (id < _tiles.size() || id == InvalidTile)
                    _map[i] = id;
                else
                    LOG_WARN("Tile '", id, "' not found -> Skipping");
            }
        }
        else
        {
            LOG_ERROR("No map specified.");
            return false;
        }

        LOG("StaticTileMap loaded");

        return true;
    }

    template <class T, class DataSetType>
    const T* BaseStaticTileMap<T, DataSetType>::get(int x, int y) const
    {
        return getTile(x / _tsize[X] - (x < 0 ? 1 : 0),
                       y / _tsize[Y] - (y < 0 ? 1 : 0));
    }

    template <class T, class DataSetType>
    const T* BaseStaticTileMap<T, DataSetType>::getTile(int x, int y) const
    {
        const TileID* t = _get(x, y);
        return !t || *t == InvalidTile ? NULL : &_tiles[*t];
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::setVirtualSize(int w, int h)
    {
        _virtsize[X] = w == noRepeat ? _size[X] : w;
        _virtsize[Y] = h == noRepeat ? _size[Y] : h;
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::setTileSize(int w, int h)
    {
        _tsize[X] = w;
        _tsize[Y] = h;
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::setDataSet(const DataSetType* set)
    {
        _dataset = set;
    }

    template <class T, class DataSetType>
    TranslationMap BaseStaticTileMap<T, DataSetType>::importDataSet(const DataSetType& set)
    {
        TranslationMap t;
        _importSet(set, set.size());
        auto seti = set.begin();
        for (size_t i = 0; i < _tiles.size(); ++i, ++seti)
        {
            _tiles[i] = seti->second;
            t[seti->first] = i;
        }
        return t;
    }

    template <class T, class DataSetType>
    TranslationMap BaseStaticTileMap<T, DataSetType>::importDataSet(const DataSetType& set, const std::vector<DataSetKey>& objs)
    {
        TranslationMap t;
        _importSet(set, objs.size());
        for (size_t i = 0; i < objs.size(); ++i)
        {
            _tiles[i] = set[objs[i]];
            t[objs[i]] = i; // TODO: overload to use rvalue references
        }
        return t;
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::setTileSet(const std::vector<T>& set)
    {
        if (set.size() < _tiles.size())
            LOG_WARN("New tileset has less elements than the old one.");
        _tiles = set;
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::setTileSet(std::vector<T>&& set)
    {
        if (set.size() < _tiles.size())
            LOG_WARN("New tileset has less elements than the old one.");
        _tiles = set;
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::set(int x, int y, TileID id)
    {
        (*_get(x, y)) = id;
    }

    template <class T, class DataSetType>
    TileID* BaseStaticTileMap<T, DataSetType>::_get(int x, int y)
    {
        return const_cast<TileID*>(static_cast<const BaseStaticTileMap<T, DataSetType>*>(this)->_get(x, y));
    }

    template <class T, class DataSetType>
    const TileID* BaseStaticTileMap<T, DataSetType>::_get(int x, int y) const
    {
        x = _adaptCoords(x, X);
        y = _adaptCoords(y, Y);
        if (x < _size[X] && y < _size[Y])
            return &_map[y * _size[X] + x];
        return NULL;
    }

    template <class T, class DataSetType>
    int BaseStaticTileMap<T, DataSetType>::_adaptCoords(int val, int index) const
    {
        if (_virtsize[index] == infiniteRepeat || val < _virtsize[index])
            return std::abs(val) % _size[index];
        return val;
    }


    template <class T, class DataSetType>
    TranslationMap BaseStaticTileMap<T, DataSetType>::_loadTileSet(const Json::Value& node)
    {
        TranslationMap t;
        const auto& objects = node["objects"];
        _tiles.resize(objects.size());
        for (Json::ArrayIndex i = 0; i < objects.size(); ++i)
        {
            auto s = objects[i].asString();
            auto it = _dataset->find(s);
            if (it != _dataset->end())
            {
                t[s] = i;
                _tiles[i] = it->second;
            }
            else
            {
                LOG_WARN("Object '", s, "' does not exist -> Skipping");
            }
        }
        return t;
    }

    template <class T, class DataSetType>
    void BaseStaticTileMap<T, DataSetType>::_importSet(const DataSetType& set, size_t tsetsize)
    {
        _dataset = &set;
        if (tsetsize < _tiles.size())
            LOG_WARN("New tileset has less elements than the old one.");
        _tiles.resize(tsetsize);
    }
}

#endif
