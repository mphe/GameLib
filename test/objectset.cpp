#include "engine/tile/ObjectSet.hpp"
#include "engine/tile/Tile.hpp"
#include <cassert>

using namespace engine;

class ObjA : public Tile
{
    public:
        bool loadFromJson(const Json::Value& node);
    public:
        static const int id;
        int x;
};

class ObjB : public Tile
{
    public:
        static Tile* create(const Tile* ptr);
        bool loadFromJson(const Json::Value& node);
    public:
        static const int id;
        int y, z;
};


int main()
{
    ObjectSet set;
    set.add<ObjA>(ObjA::id);
    set.add(ObjB::id, ObjB::create);

    assert(set.size() == 2);

    Json::Value doc;
    Json::Reader reader;
    reader.parse("{ \"42\": { \"x\": 66 }, \"85\": { \"y\": 88, \"z\": 11 } }", doc, false);

    bool success = set.loadFromJson(doc);
    assert(success == true);

    std::unique_ptr<ObjA> a(static_cast<ObjA*>(set.create(ObjA::id).release()));
    assert(a->x == 66);

    std::unique_ptr<ObjB> b(static_cast<ObjB*>(set.create(ObjB::id).release()));
    assert(b->y == 88);
    assert(b->z == 11);

    return 0;
}


bool ObjA::loadFromJson(const Json::Value& node)
{
    x = node["x"].asInt();
    return true;
}

bool ObjB::loadFromJson(const Json::Value& node)
{
    y = node["y"].asInt();
    z = node["z"].asInt();
    return true;
}

Tile* ObjB::create(const Tile* ptr)
{
    if (ptr)
        return new ObjB(*static_cast<const ObjB*>(ptr));
    return new ObjB();
}

const int ObjA::id = 42;
const int ObjB::id = 85;
