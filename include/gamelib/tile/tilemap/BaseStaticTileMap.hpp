#ifndef GAMELIB_BASE_STATIC_TILEMAP_HPP
#define GAMELIB_BASE_STATIC_TILEMAP_HPP

#include <vector>
#include "gamelib/utils/JsonObject.hpp"
#include "gamelib/DataSet.hpp"

/*
 * BaseStaticTileMap provides basic functionality (loading, accessing,
 * setting, ...) for tile maps.
 * The template parameter T declares the data stored for each tile type.
 * See StaticTileMap for an example.
 *
 * It's a static map, that means it does not store an instance of T for every
 * position but instead stores an array of Ts consisting of every tile type
 * (tileset) and another array of ints (TileIDs), representing the index
 * in the tileset, as map.
 *
 * TODO: Add more (detailed) documentation.
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
    typedef int TileID;
    typedef std::unordered_map<DataSetKey, TileID> TranslationMap;

    constexpr int InvalidTile = -1;

    constexpr int noRepeat = 0;
    constexpr int infiniteRepeat = -1;

    template <class T, class DataSetType = DataSet<T> >
    class BaseStaticTileMap : public Json::JsonObject
    {
        public:
            BaseStaticTileMap();
            virtual ~BaseStaticTileMap() {}

            // Create a new map with the given width and height.
            // If an array is passed, it will be used as the new map.
            // If the array is smaller than the actual map size, it will be
            // filled up with InvalidTile.
            void create(int w, int h);
            void create(int w, int h, const std::vector<TileID>& map);
            void create(int w, int h, std::vector<TileID>&& map);

            void destroy();

            virtual bool loadFromJson(const Json::Value& node);

            // Takes global coordinates and translates them to tile coordinates
            // to get the corresponding tile data.
            // If there's no tile at the given position, it returns NULL.
            const T* get(int x, int y) const;

            // Unlinke above, this function takes tile coordinates as input.
            const T* getTile(int x, int y) const;

            // Takes tile coordinates to set the tile at this position to the
            // given id.
            void set(int x, int y, TileID id);

            // Sets the virtual map size.
            // infiniteRepeat = repeat infinitely
            //       noRepeat = no repeat (default)
            //             >0 = repeat until the given size is reached (NOT repeat n times)
            void setVirtualSize(int w, int h);

            // Set the tile size. This effects only the get() function.
            void setTileSize(int w, int h);

            // Set the DataSet to use, but don't import anything.
            // Passing NULL will effectively disable all DataSet related
            // functionality
            void setDataSet(const DataSetType* set);

            // Set the DataSet to use and optionally a list of objects to
            // import. If no list is specified, everything is imported.
            // It returns a TranslationMap object storing DataSetKey-TileID
            // pairs describing which DataSet object is mapped to which TileID.
            TranslationMap importDataSet(const DataSetType& set);
            TranslationMap importDataSet(const DataSetType& set, const std::vector<DataSetKey>& objs);

            // Manually set the tileset without a DataSet.
            void setTileSet(const std::vector<T>& set);
            void setTileSet(std::vector<T>&& set);

        protected:
            // Returns a pointer to the position in the map at tilex/tiley
            // regarding the virtual size. If the position is outside the
            // map, NULL is returned.
            TileID* _get(int tilex, int tiley);
            const TileID* _get(int tilex, int tiley) const;

            // Applies the repeat-rules to the given tile coordinate and
            // returns the corresponding array index.
            // <val> = the coordinate's current value
            // <index> = the coordinate's index (x = 0, y = 1)
            int _adaptCoords(int val, int index) const;

            // A helper function to prevent code repetition.
            // Sets _dataset to set and resizes _tiles to tsetsize, aswell as
            // doing a size check if the current tile set is larger than the
            // new one.
            void _importSet(const DataSetType& set, size_t tsetsize);

            // Loads a tileset from json and returns a map storing
            // the DataSetKey -> TileID translations.
            TranslationMap _loadTileSet(const Json::Value& node);

        protected:
            int _size[2]; // x = 0, y = 1
            int _virtsize[2]; // same here
            int _tsize[2]; // same here
            const DataSetType* _dataset;
            std::vector<TileID> _map;
            std::vector<T> _tiles;
    };
}

#include "BaseStaticTileMap.inl"

#endif
