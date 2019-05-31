#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include <cassert>

using namespace gamelib;

class ComponentA : public Identifier<0x76d44c07, Component>
{
    public:
        ASSIGN_NAMETAG("ComponentA");
        ComponentA() {}
};

class ComponentB : public Identifier<0x650e189e, Component>
{
    public:
        ASSIGN_NAMETAG("ComponentB");
        ComponentB() {}

        bool loadFromJson(const Json::Value& node) final override
        {
            x = node.get("x", 0).asInt();
            return Component::loadFromJson(node);
        }

    public:
        int x;
};

void testEntity(Entity& entity)
{
    assert(entity.getTransform().getPosition() == math::Point2f(5, 5));
    assert(entity.getTransform().getScale() == math::Vec2f(-1.5, -1.5));
    assert(entity.getTransform().getRotation() == 90);
    assert(entity.size() == 2);
    assert(entity.findByType<ComponentA>());
    assert(entity.findByType<ComponentB>());
    assert(entity.findByType<ComponentB>()->x == 42);
}


int main()
{
    Json::Value res;
    Json::Reader().parse("{\
            \"name\": \"testentity\",\
            \"transform\": {\
                \"pos\": [ 5, 5 ],\
                \"scale\": [ -1.5, -1.5 ],\
                \"angle\": 90\
            },\
            \"components\": {\
                \"ComponentA\": {\
                },\
                \"ComponentB\": {\
                    \"x\": 42\
                }\
            }\
        }", res);

    Json::Value shortenedres;
    Json::Reader().parse("{\
            \"name\": \"testentity\",\
            \"components\": {\
                \"ComponentA\": {\
                },\
                \"ComponentB\": {\
                    \"x\": 42\
                }\
            }\
        }", shortenedres);

    Json::Value shortenedres2;
    Json::Reader().parse("{\
            \"name\": \"testentity\",\
            \"transform\": {\
                \"scale\": [ -1.5 ]\
            },\
            \"components\": {\
                \"ComponentA\": {\
                },\
                \"ComponentB\": {\
                    \"x\": 42\
                }\
            }\
        }", shortenedres2);


    EntityFactory factory;
    factory.addComponent<ComponentA>("ComponentA");
    factory.addComponent<ComponentB>("ComponentB");
    factory.add(res);

    Entity entity;
    factory.create("testentity", &entity);
    testEntity(entity);

    factory.removeEntity("testentity");

    entity.destroy();
    factory.createFromJson(res, &entity);
    testEntity(entity);

    entity.destroy();
    factory.createFromJson(shortenedres, &entity);
    // no special tests, just to see if it handles incomplete configs correctly

    entity.destroy();
    factory.createFromJson(shortenedres2, &entity);
    // no special tests, just to see if it handles incomplete configs correctly

    return 0;
}
