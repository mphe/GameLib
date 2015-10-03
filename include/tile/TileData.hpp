#ifndef TILE_DATA_HPP
#define TILE_DATA_HPP

namespace engine
{
    typedef int TileID;
    typedef int TileType;
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
                type(InvalidTile)
            { }

        public:
            TileID id;
            TileType type;
            TexData texdata;
            //collision mask
    };
}

#endif
