#include "gamelib/core/IDCounter.hpp"
#include "json/json.h"
#include <cassert>

using namespace gamelib;

class Foo : public IDCounter<Foo>
{
    public:
        using IDCounter::_invalidateID;
};

class Bar : public IDCounter<Bar, 10> { };
class Global1 : public GlobalIDCounter { };
class Global2 : public GlobalIDCounter { };

int main(int argc, char *argv[])
{
    Foo foos[3];
    for (size_t i = 0; i < 3; ++i)
        assert(foos[i].getID() == i && "Wrong ID");

    Bar bars[3];
    for (size_t i = 0; i < 3; ++i)
        assert(bars[i].getID() == Bar::IDOffset + i && "Wrong ID");

    foos[0]._invalidateID();
    assert(foos[0].getID() == invalidID && "Wrong ID");

    Global1 globals[3];
    Global2 global2s[3];

    for (size_t i = 0; i < 3; ++i)
        assert(globals[i].getID() == i && "Wrong ID");
    for (size_t i = 0; i < 3; ++i)
        assert(global2s[i].getID() == 3 + i && "Wrong ID");

    Json::Value node;
    node["uid"] = 10000;

    Global1 g;
    g.loadFromJson(node);
    assert(g.getID() == 10000 && "Wrong ID after loading");

    Global2 g2;
    assert(g2.getID() == 10001 && "Wrong ID");

    node.clear();
    g2.writeToJson(node);
    assert(node["uid"].asUInt() == 10001 && "Wrong json output");

    return 0;
}
