#include <cassert>
#include <memory>
#include <iostream>
#include "gamelib/utils/SlotMap.hpp"

using namespace gamelib;
using namespace std;

struct Foo
{
    public:
        int x, y, z;
        static bool control;

        ~Foo()
        {
            control = true;
        }
};

bool Foo::control = false;

int main()
{
    SlotMapShort<std::unique_ptr<Foo>> pmap;
    auto pkey = pmap.acquire();
    pmap[pkey] = std::unique_ptr<Foo>(new Foo());
    pmap.destroy(pkey);

    assert(Foo::control && "Object didn't get overwritten");

    SlotMapShort<Foo> map(5);
    static_assert(sizeof(SlotMapShort<Foo>::Handle) == 2 * sizeof(unsigned short),
            "Wrong key size");

    auto key = map.acquire();

    assert(key.index == 0 && "Wrong index");
    assert(key.version == 0 && "Wrong version tag");
    assert(map.isValid(key) && "Key should be valid");

    auto& foo = map[key];
    foo.x = foo.y = foo.z = 42;

    auto key2 = map.acquire();

    assert(key2.index == 1 && "Wrong index");
    assert(key2.version == 0 && "Wrong version tag");
    assert(map.isValid(key2) && "Key should be valid");
    // assert(map.size() == 2 && "Wrong size");

    int i = 0;
    for (auto it = map.begin(), end = map.end(); it != end; ++it)
    {
        assert(it.handle().index == i && "Wrong index");;
        assert(it.handle().version == 0 && "Wrong version tag");;
        ++i;
    }

    map.destroy(key);

    assert(!map.isValid(key) && "Key should be invalid");

    key = map.acquire();

    assert(key.index == 0 && "Wrong index");
    assert(key.version == 1 && "Wrong version tag");
    assert(map.isValid(key) && "Key should be valid");

    for (int i = 0; i < 10; ++i)
        map.acquire();

    int num = 0;
    for (auto it = map.begin(), end = map.end(); it != end; ++it)
        num++;

    assert(num == 12 && "Wrong size");

    map.clear();

    num = 0;
    for (auto& i : map)
        num++;

    assert(num == 0 && "Wrong size");

    return 0;
}
