#include <cassert>
#include <string>
#include <iostream>
#include "gamelib/properties/Property.hpp"
#include "gamelib/properties/basictypes.hpp"
#include "math/geometry/Vector.hpp"

using namespace gamelib;
using namespace std;

struct SomeClass
{
    int x;
    int y;

    static void setXNice(int* var, const int* value, SomeClass* self)
    {
        self->x = *value;
        self->y = *var;
    };
};

struct SomeClassSerializer : public PropType<0x13a51995, SomeClass>
{
    auto loadFromJson(const PropertyHandle& prop, SomeClass* ptr, const Json::Value& node) const -> bool final override
    {
        ptr->x = node["x"].asInt();
        ptr->y = node["y"].asInt();
        return true;
    }

    auto writeToJson(const PropertyHandle& prop, const SomeClass* ptr, Json::Value& node) const -> void final override
    {
        node["x"] = prop.getAs<int>();
        node["y"] = prop.getAs<int>();
    }

    auto drawGui(const PropertyHandle& prop, const std::string& name, SomeClass* ptr) const -> bool final override
    {
        return false;
    }
} someClassSerializer;


int main(int argc, char *argv[])
{
    PropertyContainer props;
    float somefloat = 5.5;
    int someint = 42;
    math::Vec2i somevec2i(8, 8);
    std::string somestring = "test";
    SomeClass someobject { 0, 0 };

    props.registerProperty("somefloat", somefloat);
    props.registerProperty("someint", someint, 0, 100);
    props.registerProperty("somevec2i", somevec2i);
    props.registerProperty("somestring", somestring);
    props.registerProperty("someobject", someobject.x, SomeClass::setXNice, &someobject, &someClassSerializer);

    assert(props.size() == 5 && "Wrong size");

    for (auto i : { "somefloat", "someint", "somevec2i", "somestring", "someobject" })
        assert(props.get(i) != nullptr && "Missing key");

    assert(*props.getAs<float>("somefloat")       == 5.5f && "Wrong value");
    assert(*props.getAs<int>("someint")           == 42 && "Wrong value");
    assert(*props.getAs<math::Vec2i>("somevec2i") == math::Vec2i(8, 8) && "Wrong value");
    assert(*props.getAs<string>("somestring")     == "test" && "Wrong value");
    assert(*props.getAs<int>("someobject")        == 0 && "Wrong value");

    assert(props.find("somefloat")->serializer  == &propFloat && "Wrong type auto-detected");
    assert(props.find("someint")->serializer    == &propInt && "Wrong type auto-detected");
    assert(props.find("somevec2i")->serializer  == &propVec2i && "Wrong type auto-detected");
    assert(props.find("somestring")->serializer == &propString && "Wrong type auto-detected");
    assert(props.find("someobject")->serializer == &someClassSerializer && "Wrong custom serializer");

    assert(!props.get("somevar") && "Property should not exist");

    // Test direct set
    props.set<float>("somefloat", 7.4);
    assert(*props.getAs<float>("somefloat") == 7.4f && "Wrong value");

    // Test setters
    props.set<int>("someobject", 5);
    assert(*props.getAs<int>("someobject") == 5 && "Wrong value");
    assert(someobject.x == someobject.y && "Wrong value");

    // Test unregistering
    props.unregisterProperty("someobject");
    assert(props.size() == 4 && "Wrong size");
    assert(props.find("someobject") == nullptr && "Property should not exist");

    // Test nice setter
    props.registerProperty("someobject", someobject.x, SomeClass::setXNice, &someobject, &someClassSerializer);
    props.set<int>("someobject", 10);
    assert(*props.getAs<int>("someobject") == 10 && "Wrong value");
    assert(someobject.x == someobject.y && "Wrong value");


    // Test serialization
    Json::Value val;
    props.writeToJson(val);
    cout<<val.toStyledString()<<endl;

    auto tmpsomefloat = somefloat;
    auto tmpsomeint = someint;
    auto tmpsomevec2i = somevec2i;
    auto tmpsomestring = somestring;
    auto tmpsomeclass = someobject;

    somefloat = 0;
    someint = 0;
    somevec2i.fill(0);
    somestring = "";
    someobject = {0, 0};

    assert(*props.getAs<float>("somefloat")       == 0.f && "Wrong value");
    assert(*props.getAs<int>("someint")           == 0 && "Wrong value");
    assert(*props.getAs<math::Vec2i>("somevec2i") == math::Vec2i() && "Wrong value");
    assert(*props.getAs<string>("somestring")     == "" && "Wrong value");
    assert(*props.getAs<int>("someobject")        == 0 && "Wrong value");

    props.loadFromJson(val);

    assert(math::almostEquals(*props.getAs<float>("somefloat"), tmpsomefloat) && "Wrong value");
    assert(*props.getAs<int>("someint")           == tmpsomeint && "Wrong value");
    assert(*props.getAs<math::Vec2i>("somevec2i") == tmpsomevec2i && "Wrong value");
    assert(*props.getAs<string>("somestring")     == tmpsomestring && "Wrong value");
    assert(*props.getAs<int>("someobject")        == tmpsomeclass.x && "Wrong value");
    assert(someobject.y                           == tmpsomeclass.y && "Wrong value");

    props.clear();

    assert(props.size() == 0 && "Wrong size");

    return 0;
}
