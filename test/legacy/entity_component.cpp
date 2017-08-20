#include "gamelib/component/Entity.hpp"
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

        void setEntity(Entity* ptr)
        {
            entity = ptr;
            if (ptr != nullptr)
            {
                FooComponent* foo = ptr->get<FooComponent>();
                assert("FooComponent not found" && foo);
                x = foo->x;
            }
        }

    public:
        Entity* entity;
        int x;
};

class NotAComponent {};


int main(int argc, char *argv[])
{
    Entity entity;

    entity.add(ComponentPtr(new FooComponent(42)));
    entity.add(ComponentPtr(new BarComponent(33)));
    entity.add(ComponentPtr(new FooComponent(88)));
    assert("Wrong size" && entity.size() == 3);

    assert("Component not found" && entity.get(FooComponent::id) != nullptr);
    assert("Component not found" && entity.get(BarComponent::id) != nullptr);
    assert("Wrong x value" && entity.get<BarComponent>()->x == 42);
    // auto ptr = entity.get<NotAComponent>(); // This should produce a compiler error

    entity.remove(BarComponent::id);
    assert("Wrong size" && entity.size() == 2);
    assert("Component not removed" && entity.get(BarComponent::id) == nullptr);

    entity.remove<FooComponent>();
    assert("Wrong size" && entity.size() == 1);
    assert("Wrong component removed" && entity.get<FooComponent>()->x == 88);

    entity.clear();
    assert("Wrong size" && entity.size() == 0);

    return 0;
}
