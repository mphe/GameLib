#ifndef GAMELIB_STATIC_TILEMAP_HPP
#define GAMELIB_STATIC_TILEMAP_HPP

#include <vector>

/*
 * StaticTilemap provides basic functionality (accessing, setting, ...)
 * for tile maps.
 * The template parameter T declares the data stored for each tile type.
 * See StaticRenderTilemap for an example.
 *
 * It's a static map, that means it does not store an instance of T for every
 * position but instead stores an array of Ts consisting of every tile type
 * (tileset) and another array of ints (TileIDs), representing the index
 * in the tileset, as map.
 *
 * TODO: Add more (detailed) documentation.
 */

namespace gamelib
{
    typedef int TileID;

    constexpr int InvalidTile = -1;

    constexpr int noRepeat = 0;
    constexpr int infiniteRepeat = -1;

    template <class>
    class TilemapCollider;

    template <class T>
    class StaticTilemap
    {
        template <class>
        friend class TilemapCollider;

        public:
            StaticTilemap();
            virtual ~StaticTilemap() {}

            // Create a new map with the given width and height.
            // If an array is passed, it will be used as the new map.
            // If the array is smaller than the actual map size, it will be
            // filled up with InvalidTile.
            void create(int w, int h);
            void create(int w, int h, const std::vector<TileID>& map);
            void create(int w, int h, std::vector<TileID>&& map);

            virtual void destroy();

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

            // Manually set the tileset without a DataSet.
            void setTileSet(const std::vector<T>& set);
            void setTileSet(std::vector<T>&& set);

            void globalToTileCoords(int* x, int* y) const;
            void tileToGlobalCoords(int* x, int* y) const;

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

        protected:
            int _size[2]; // x = 0, y = 1
            int _virtsize[2]; // same here
            int _tsize[2]; // same here
            std::vector<TileID> _map;
            std::vector<T> _tiles;
    };
}

#include "StaticTilemap.inl"

#endif
