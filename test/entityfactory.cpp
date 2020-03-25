#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
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

        bool loadFromJson(const Json::Value& node) final
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
    // Temporary store json strings
    Json::Value tmp;

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
        }", tmp);
    auto res = EntityResource::create(tmp);

    Json::Reader().parse("{\
            \"name\": \"testentity\",\
            \"components\": {\
                \"ComponentA\": {\
                },\
                \"ComponentB\": {\
                    \"x\": 42\
                }\
            }\
        }", tmp);
    auto shortenedres = EntityResource::create(tmp);

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
        }", tmp);
    auto shortenedres2 = EntityResource::create(tmp);


    EntityManager manager;
    EntityFactory factory;
    factory.addComponent<ComponentA>("ComponentA");
    factory.addComponent<ComponentB>("ComponentB");
    factory.add(res);

    Entity entity;
    factory.create("testentity", &entity);
    testEntity(entity);

    entity.destroy();
    factory.createFromJson(res->getConfig(), &entity);
    testEntity(entity);

    entity.destroy();
    factory.createFromJson(shortenedres->getConfig(), &entity);
    // no special tests, just to see if it handles incomplete configs correctly

    entity.destroy();
    factory.createFromJson(shortenedres2->getConfig(), &entity);
    // no special tests, just to see if it handles incomplete configs correctly

    entity.destroy();

    // Check error handling
    assert(!factory.create("asdaf", &entity) && "should return nullptr");

    // test with EntityManager
    auto ent = factory.create("testentity");
    assert(ent && "ent should be valid");
    testEntity(*ent);
    ent->destroy();

    factory.removeEntity("testentity");

    ent = factory.create("testentity");
    assert(!ent && "ent should be null");

    ent = factory.create("asdaf");
    assert(!ent && "ent should be null");

    return 0;
}
