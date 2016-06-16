#ifndef GAMELIB_STATIC_DATASET_TILEMAP_INL
#define GAMELIB_STATIC_DATASET_TILEMAP_INL

#include "StaticDataSetTilemap.hpp"

namespace gamelib
{
    template <class T, class DataSetType>
    StaticDataSetTilemap<T, DataSetType>::StaticDataSetTilemap(const DataSetType& dset) :
        _dataset(&dset)
    { }

    template <class T, class DataSetType>
    bool StaticDataSetTilemap<T, DataSetType>::loadFromJson(const Json::Value& node)
    {
        //meta data
        this->create(node.get("width", 0).asInt(),
                node.get("height", 0).asInt());

        this->setVirtualSize(node.get("virtwidth", noRepeat).asInt(),
                node.get("virtheight", noRepeat).asInt());

        this->setTileSize(node.get("tilew", 0).asInt(),
                node.get("tileh", 0).asInt());

        LOG_DEBUG(LOG_DUMP(this->_virtsize[X]), ", ", LOG_DUMP(this->_virtsize[Y]));
        LOG_DEBUG(LOG_DUMP(this->_size[X]), ", ", LOG_DUMP(this->_size[Y]));
        LOG_DEBUG(LOG_DUMP(this->_tsize[X]), ", ", LOG_DUMP(this->_tsize[Y]));

        if (!this->_size[X] || !this->_size[Y] || !this->_tsize[X] || !this->_tsize[Y])
        {
            LOG_ERROR("Map size or tile size is 0.");
            return false;
        }

        TranslationMap trans;
        if (!node.isMember("objects") || node["objects"].size() == 0)
            // Import all -> import from current set again
            trans = importDataSet(*_dataset);
        else
            trans = _loadTileSet(node);

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
            for (Json::ArrayIndex i = 0; i < this->_map.size() && i < tiles.size(); ++i)
            {
                TileID id = InvalidTile;
                if (tiles[i].isNumeric())
                    id = tiles[i].asInt();
                else if (trans.find(tiles[i].asString()) != trans.end())
                    id = trans.find(tiles[i].asString())->second;

                if (id < this->_tiles.size() || id == InvalidTile)
                    this->_map[i] = id;
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
    void StaticDataSetTilemap<T, DataSetType>::setDataSet(const DataSetType& set)
    {
        _dataset = &set;
    }

    template <class T, class DataSetType>
    TranslationMap StaticDataSetTilemap<T, DataSetType>::importDataSet(const DataSetType& set)
    {
        TranslationMap t;
        _importSet(set, set.size());
        auto seti = set.begin();
        for (size_t i = 0; i < this->_tiles.size(); ++i, ++seti)
        {
            this->_tiles[i] = seti->second;
            t[seti->first] = i;
        }
        return t;
    }

    template <class T, class DataSetType>
    TranslationMap StaticDataSetTilemap<T, DataSetType>::importDataSet(const DataSetType& set, const std::vector<DataSetKey>& objs)
    {
        TranslationMap t;
        _importSet(set, objs.size());
        for (size_t i = 0; i < objs.size(); ++i)
        {
            this->_tiles[i] = set[objs[i]];
            t[objs[i]] = i; // TODO: overload to use rvalue references
        }
        return t;
    }


    template <class T, class DataSetType>
    TranslationMap StaticDataSetTilemap<T, DataSetType>::_loadTileSet(const Json::Value& node)
    {
        TranslationMap t;
        const auto& objects = node["objects"];
        this->_tiles.resize(objects.size());
        for (Json::ArrayIndex i = 0; i < objects.size(); ++i)
        {
            auto s = objects[i].asString();
            auto it = _dataset->find(s);
            if (it != _dataset->end())
            {
                t[s] = i;
                this->_tiles[i] = it->second;
            }
            else
            {
                LOG_WARN("Object '", s, "' does not exist -> Skipping");
            }
        }
        return t;
    }

    template <class T, class DataSetType>
    void StaticDataSetTilemap<T, DataSetType>::_importSet(const DataSetType& set, size_t tsetsize)
    {
        _dataset = &set;
        if (tsetsize < this->_tiles.size())
            LOG_WARN("New tileset has less elements than the old one.");
        this->_tiles.resize(tsetsize);
    }
}

#endif
