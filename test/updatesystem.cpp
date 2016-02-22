#include <cassert>
#include <vector>
#include "gamelib/component/subsystem/UpdateSystem.hpp"
#include "gamelib/component/UpdateComponent.hpp"
#include "gamelib/Identifier.hpp"

using namespace gamelib;

// Global is ok for testing purposes
std::vector<UpdateComponent*> compOrder;

class FooComponent : public Identifier<0xbeefbeef, UpdateComponent>
{
    public:
        FooComponent() : x(0) {}

        void update(float fps)
        {
            x = 5;
            compOrder.push_back(this);
        }

    public:
        int x;
};

class BarComponent : public Identifier<0xdeadbeef, UpdateComponent>
{
    public:
        void update(float fps)
        {
            compOrder.push_back(this);
            destroy(); // unregister from subsys
        }
};

int main(int argc, char *argv[])
{
    UpdateSystem us;
    FooComponent foo, foo2;
    BarComponent bar;

    us.add(&foo2, 10);
    us.add(&foo, -10);
    us.add(&bar, 0);
    assert(us.size() == 3 && "Wrong size after adding components.");

    us.update(60);

    assert(compOrder[0] == &foo &&
           compOrder[1] == &bar &&
           compOrder[2] == &foo2 &&
           "Components are in the wrong order.");

    // Bar should remove itself
    assert(us.size() == 2 && "Wrong size after destroy().");

    assert(foo.x == 5 && foo2.x == 5 && "Components have not been updated.");

    us.remove(&foo);
    assert(us.size() == 1 && "Wrong size after removal.");

    return 0;
}
