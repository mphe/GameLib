#include "gamelib/ecs/Entity.hpp"
#include <cassert>

using namespace gamelib;

class FooComponent : public Identifier<0x76d44c07, Component>
{
    public:
        FooComponent(int x_) : x(x_) {}
        int x;
};

class BarComponent : public Identifier<0x650e189e, Component>
{
    public:
        BarComponent(int x_) : x(x_) {}

        void _refresh()
        {
            FooComponent* foo = getEntity()->find<FooComponent>();
            assert(foo && "FooComponent not found");
            x = foo->x;
        }

    public:
        int x;
};

class NotAComponent {};


int main(int argc, char *argv[])
{
    Entity entity;

    entity.add(ComponentPtr(new FooComponent(42)));
    auto bar = entity.add(ComponentPtr(new BarComponent(33)));
    entity.add(ComponentPtr(new FooComponent(88)));
    assert("Wrong size" && entity.size() == 3);

    assert("Component not found" && entity.find(FooComponent::id) != nullptr);
    assert("Component not found" && entity.find(BarComponent::id) != nullptr);
    assert("Wrong x value" && entity.find<BarComponent>()->x == 42);
    // auto ptr = entity.find<NotAComponent>(); // This should produce a compiler error

    entity.remove(bar);
    assert("Wrong size" && entity.size() == 2);
    assert("Component not removed" && entity.find(BarComponent::id) == nullptr);

    entity.remove(entity.find<FooComponent>());
    assert("Wrong size" && entity.size() == 1);
    assert("Wrong component removed" && entity.find<FooComponent>()->x == 88);

    entity.destroy();
    assert("Wrong size" && entity.size() == 0);

    return 0;
}
