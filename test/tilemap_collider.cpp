#include <cassert>
#include <time.h>
#include "gamelib/tile/StaticTilemap.hpp"
#include "gamelib/tile/TilemapCollider.hpp"

using namespace std;
using namespace gamelib;

struct Tile
{
    int x;
};

int main(int argc, char *argv[])
{
    srand(time(NULL));

    StaticTilemap<Tile> map;
    map.setTileSize(32, 32);
    map.setTileSet({ Tile{0}, Tile{1} });
    map.create(6, 5, { 1, 1, 1, 1, 1, 1,
                       1, 0, 0, 0, 0, 1,
                       1, 0, 0, 0, 0, 1,
                       1, 0, 0, 0, 0, 1,
                       1, 1, 1, 1, 1, 1 });

    TilemapCollider<Tile> tcol(map,
            [](const Tile& t) { return t.x == 1; });

    math::AABB<float> rect;

    for (size_t i = 0; i < 100; ++i)
    {
        rect.pos.x = random() % 640 - random() % 640;
        rect.pos.y = random() % 640 - random() % 640;
        rect.size.x = random() % 640;
        rect.size.y = random() % 640;

        if (rect.intersects(tcol.getBBox()) &&
                (rect.pos.x < 32 || rect.pos.x + rect.size.x > 5 * 32 ||
                rect.pos.y < 32 || rect.pos.y + rect.size.y > 4 * 32))
            assert("rect should intersect" && tcol.intersects(rect));
        else
            assert("rect should not intersect" && !tcol.intersects(rect));
    }

    return 0;
}
