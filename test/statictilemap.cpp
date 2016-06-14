#include "gamelib/tile/tilemap/BaseStaticTileMap.hpp"
#include <cassert>
#include "gamelib/utils/JsonObject.hpp"

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

    BaseStaticTileMap<Tile> map;
    auto trans = map.importDataSet(set);
    map.create(10, 15, { trans["tile1"], trans["tile2"],
            trans["tile3"], trans["tile4"] });
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

    map.create(2, 2, { trans["tile1"], trans["tile2"],
            trans["tile3"], trans["tile4"] });
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
