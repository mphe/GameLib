#include <cassert>
#include "gamelib/component/subsystem/UpdateSystem.hpp"
#include "gamelib/component/UpdateComponent.hpp"
#include "gamelib/Identifier.hpp"

using namespace gamelib;

class FooComponent : public Identifier<0xbeefbeef, UpdateComponent>
{
    public:
        FooComponent() : x(0) {}

        void update(float fps)
        {
            x = 5;
        }

    public:
        int x;
};

class BarComponent : public Identifier<0xdeadbeef, UpdateComponent>
{
    public:
        void update(float fps)
        {
            destroy(); // unregister from subsys
        }
};

int main(int argc, char *argv[])
{
    UpdateSystem us;
    FooComponent foo, foo2;
    BarComponent bar;

    us.add(&foo);
    us.add(&bar);
    us.add(&foo2);
    assert(us.size() == 3 && "Wrong size after adding components.");

    us.update(60);

    // Bar should remove itself
    assert(us.size() == 2 && "Wrong size after destroy().");

    assert(foo.x == 5 && foo2.x == 5 && "Components have not been updated.");

    us.remove(&foo);
    assert(us.size() == 1 && "Wrong size after removal.");

    return 0;
}
