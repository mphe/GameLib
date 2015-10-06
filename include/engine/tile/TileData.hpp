#ifndef TILE_DATA_HPP
#define TILE_DATA_HPP

namespace engine
{
    // TODO: Merge into Tile?

    typedef int TileID;
    constexpr TileID InvalidTile = -1;

    struct TexData
    {
        int index;
        int length;
        float speed;
        float offset;
    };

    class TileData
    {
        public:
            TileData() :
                id(InvalidTile),
                flags(0)
            { }

        public:
            TileID id;
            int flags;
            TexData texdata;
            //collision mask
    };
}

#endif
