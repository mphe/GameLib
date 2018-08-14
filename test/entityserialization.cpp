#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include <cassert>

using namespace gamelib;

class ComponentA : public Identifier<0x76d44c07, Component>
{
    public:
        ComponentA() : Identifier("ComponentA") { _props.registerProperty("x", x); }
        int x;
};

class ComponentB : public Identifier<0x650e189e, Component>
{
    public:
        ComponentB() : Identifier("ComponentB") { _props.registerProperty("x", x); }
        int x;
};


Component* find(Entity& ent, const std::string& name, unsigned int id)
{
    for (auto it = ent.begin(), end = ent.end(); it != end; ++it)
        if (it->id == id && it->ptr->getName() == name)
            return it->ptr.get();
    return nullptr;
}


int main()
{
    Json::Value res;
    Json::Reader().parse("{\
            \"name\": \"testentity\",\
            \"components\": {\
                \"ComponentA#\": {\
                    \"x\": 1\
                },\
                \"ComponentA#2\": {\
                    \"x\": 2\
                },\
                \"ComponentB\": {\
                    \"x\": 1\
                },\
                \"ComponentB#42\": {\
                    \"x\": 42\
                }\
            }\
        }", res);

    Json::Value res2;
    Json::Reader().parse("{\
            \"name\": \"testentity\",\
            \"components\": {\
                \"ComponentA#\": {\
                    \"x\": 1\
                },\
                \"ComponentB#10\": {\
                    \"x\": 2\
                },\
                \"ComponentB#42\": {\
                    \"x\": 42\
                }\
            }\
        }", res2);



    EntityFactory factory;
    factory.addComponent<ComponentA>("ComponentA");
    factory.addComponent<ComponentB>("ComponentB");
    factory.add(res);

    Entity entity;
    factory.create("testentity", &entity);

    assert(entity.size() == 4);
    assert(find(entity, "ComponentA", 1) != nullptr && "Missing component or wrong ID");
    assert(find(entity, "ComponentA", 2) != nullptr && "Missing component or wrong ID");
    assert(find(entity, "ComponentB", 1) != nullptr && "Missing component or wrong ID");
    assert(find(entity, "ComponentB", 42) != nullptr && "Missing component or wrong ID");

    for (auto it = entity.begin(), end = entity.end(); it != end; ++it)
    {
        unsigned int val = *it->ptr->getProperties().getAs<int>("x");
        assert(it->id == val && "Incorrect property value or ID");
        it->ptr->getProperties().find("x")->set<int>(random());
    }

    Json::Value out;
    writeToJson(out, entity);

    Entity ent2;
    loadFromJson(out, ent2);

    for (auto it = ent2.begin(), end = ent2.end(); it != end; ++it)
    {
        auto comp = find(entity, it->ptr->getName(), it->id);
        assert(comp != nullptr && "Incorrectly loaded component");

        unsigned int val = *it->ptr->getProperties().getAs<int>("x");
        assert(*comp->getProperties().getAs<int>("x") == val && "Wrong value");
    }


    // Test serialization with changing entity/component config
    factory.removeEntity("testentity");
    factory.add(res2);
    factory.createWithDelta("testentity", out, &ent2);

    assert(ent2.size() == 3);
    assert(find(ent2, "ComponentA", 1) != nullptr && "Missing component or wrong ID");
    assert(find(ent2, "ComponentB", 10) != nullptr && "Missing component or wrong ID");
    assert(find(ent2, "ComponentB", 42) != nullptr && "Missing component or wrong ID");

    int a, b1, b2;
    a = static_cast<ComponentA*>(find(ent2, "ComponentA", 1))->x;
    b1 = static_cast<ComponentB*>(find(ent2, "ComponentB", 10))->x;
    b2 = static_cast<ComponentB*>(find(ent2, "ComponentB", 42))->x;

    int ta, tb;
    ta = static_cast<ComponentA*>(find(entity, "ComponentA", 1))->x;
    tb = static_cast<ComponentB*>(find(entity, "ComponentB", 42))->x;

    assert(b1 == 2 && "Wrong value");
    assert(a == ta && "Wrong value");
    assert(b2 == tb && "Wrong value");

    return 0;
}
