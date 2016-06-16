#ifndef GAMELIB_STATIC_DATASET_TILEMAP_HPP
#define GAMELIB_STATIC_DATASET_TILEMAP_HPP

#include "StaticTilemap.hpp"
#include "gamelib/utils/JsonObject.hpp"
#include "gamelib/DataSet.hpp"

/*
 * Extends StaticDataSetTilemap with support for DataSets and loading
 * from json files.
 *
 * The first template parameter is the same as in StaticTilemap and
 * defines the tile data to store.
 * The second parameter is the DataSet type to use.
 * This is useful in cases where a derived class might need its own
 * implementation of a DataSet, e.g. SpriteSet and StaticRenderTilemap.
 *
 * There is no static type checking, that means there will be
 * the usual compiler errors, if the given type does not fit the
 * criteria for a DataSet (e.g. not having a certain function).
 *
 * Config file structure:
 * (Lines starting with # are comments and are not valid json.)
 *
 * {
 *      # Tile size (width, height)
 *      "tilew": <int>,
 *      "tileh": <int>,
 *
 *      # Map size (width, height)
 *      "width": <int>,
 *      "height": <int>,
 *
 *      # (Optional) Virtual map size
 *      # -1 = repeat infinitely
 *      #  0 = no repeat (default)
 *      # >0 = repeat until the given size is reached (NOT repeat n times)
 *      "virtwidth": <int>,
 *      "virtheight": <int>,
 *
 *      # (Optional) A list of objects to import from the DataSet.
 *      # If not specified, all objects are imported.
 *      "objects": [ <string>, <string>, ... ],
 *
 *      # (Optional) A list of aliases that map to a object name or tile ID
 *      "alias": {
 *          "<string>": <int/string>,
 *          ...
 *      },
 *
 *      # The actual map, defined by aliases, tile IDs, or object names.
 *      # Use -1 for an empty tile.
 *      # Missing tiles are filled up with -1.
 *      "map": [<string/int>, ...]
 * }
 */

namespace gamelib
{
    typedef std::unordered_map<DataSetKey, TileID> TranslationMap;

    template <class T, class DataSetType = DataSet<T> >
    class StaticDataSetTilemap : public StaticTilemap<T>, public Json::JsonObject
    {
        public:
            // This does not import dset. It's equal to calling setDataSet.
            StaticDataSetTilemap(const DataSetType& dset);
            virtual ~StaticDataSetTilemap() {}

            virtual bool loadFromJson(const Json::Value& node);

            // Set the DataSet to use, but don't import anything.
            void setDataSet(const DataSetType& dset);

            // Set the DataSet to use and optionally a list of objects to
            // import. If no list is specified, everything is imported.
            // It returns a TranslationMap storing DataSetKey-TileID pairs
            // describing which TileID is mapped to which object.
            TranslationMap importDataSet(const DataSetType& set);
            TranslationMap importDataSet(const DataSetType& set, const std::vector<DataSetKey>& objs);

        protected:
            // A helper function to prevent code repetition.
            // Sets _dataset to set and resizes _tiles to tsetsize, aswell as
            // doing a size check if the current tile set is larger than the
            // new one.
            void _importSet(const DataSetType& set, size_t tsetsize);

            // Loads a tileset from json and returns a map storing
            // the DataSetKey -> TileID translations.
            TranslationMap _loadTileSet(const Json::Value& node);

        protected:
            const DataSetType* _dataset;
    };
}

#include "StaticDataSetTilemap.inl"

#endif
