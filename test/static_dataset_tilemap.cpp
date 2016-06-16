#include "gamelib/tile/StaticDataSetTilemap.hpp"
#include <cassert>

using namespace gamelib;

struct Tile
{
    int x;
};

int main(int argc, char *argv[])
{
    DataSet<Tile> set;
    set["tile1"] = Tile{ 1 };
    set["tile2"] = Tile{ 2 };
    set["tile3"] = Tile{ 3 };
    set["tile4"] = Tile{ 4 };

    StaticDataSetTilemap<Tile> map(set);

    Json::Value root;
    Json::Reader().parse("{\n\
            \"tilew\": 32,\n\
            \"tileh\": 32,\n\
            \"width\": 5,\n\
            \"height\": 3,\n\
            \"virtwidth\": 25,\n\
            \"virtheight\": -1,\n\
            \"objects\": [ \"tile1\", \"tile2\", \"tile3\", \"tile4\" ],\n\
            \"alias\": {\n\
                \"a\": 0,\n\
                \"b\": 1,\n\
                \"c\": \"tile3\",\n\
                \"d\": 3\n\
                },\n\
            \"map\": [ \"a\", \"b\", \"c\", \"d\", -1,\n\
                      -1, \"d\", \"c\", \"b\", \"a\",\n\
                      -1,-1,-1,-1,-1 ]\n\
            }", root);

    assert("Failed to load test json" && map.loadFromJson(root));

    const int compare[3][5] = { 1, 2, 3, 4, 0,
                                0, 4, 3, 2, 1,
                                0, 0, 0, 0, 0 };

    for (size_t y = 0; y < 15; ++y)
    {
        for (size_t x = 0; x < 30; ++x)
        {
            const Tile* t = map.get(x * 32 + 5, y * 32 + 5);
            if (compare[y % 3][x % 5] == 0 || x > 24)
                assert("Expected InvalidTile" && t == NULL);
            else
                assert("Unexpected tile data" &&
                        t != NULL && t->x == compare[y % 3][x % 5]);
        }
    }

    return 0;
}
