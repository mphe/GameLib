#include "gamelib/core/tile/StaticTilemap.hpp"
#include <cassert>

using namespace gamelib;

struct Tile
{
    int x;
};

int main(int argc, char *argv[])
{
    StaticTilemap<Tile> map;
    map.setTileSet({ Tile{1}, Tile{2}, Tile{3}, Tile{4} });
    map.create(10, 15, { 0, 1, 2, 3 });

    for (size_t i = 0; i < 4; ++i)
    {
        const Tile* t = map.getTile(i, 0);
        assert("Unexpected tile data" && t != NULL && t->x == i + 1);
        map.set(i, 0, InvalidTile);
    }

    for (size_t y = 0; y < 15; ++y)
        for (size_t x = 0; x < 10; ++x)
            assert("Unexpected tile data, expected InvalidTile." &&
                    map.getTile(x, y) == NULL);

    map.create(2, 2, { 0, 1, 2, 3 });
    map.setVirtualSize(infiniteRepeat, infiniteRepeat);

    for (size_t y = 0; y < 15; ++y)
    {
        for (size_t x = 0; x < 10; ++x)
        {
            const Tile* t = map.getTile(x, y);
            assert("Unexpected tile data, expected InvalidTile." &&
                    t != NULL && (y % 2 ? t->x == 3 + (x % 2) :
                        t->x == 1 + (x % 2) ));
        }
    }

    return 0;
}
