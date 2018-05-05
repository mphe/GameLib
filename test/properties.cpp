#include <cassert>
#include <string>
#include <iostream>
#include "gamelib/utils/Property.hpp"

using namespace gamelib;
using namespace std;

struct SomeClass
{
    int x;
    int y;

    static void setX(void* var, const void* value, void* self_)
    {
        SomeClass* self = (SomeClass*)self_;
        self->x = *(int*)value;
        self->y = self->x;
    };

    static void setXNice(int* var, const int* value, SomeClass* self)
    {
        self->x = *value;
        self->y = *var;
    };
};

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
    props.registerProperty("someobject", someobject.x, SomeClass::setX, &someobject);

    assert(props.size() == 5 && "Wrong size");

    for (auto i : { "somefloat", "someint", "somevec2i", "somestring", "someobject" })
        assert(props.get(i) != nullptr && "Missing key");

    assert(*props.getAs<float>("somefloat")       == 5.5f && "Wrong value");
    assert(*props.getAs<int>("someint")           == 42 && "Wrong value");
    assert(*props.getAs<math::Vec2i>("somevec2i") == math::Vec2i(8, 8) && "Wrong value");
    assert(*props.getAs<string>("somestring")     == "test" && "Wrong value");
    assert(*props.getAs<int>("someobject")        == 0 && "Wrong value");

    assert(props.find("somefloat")->type  == PropFloat && "Wrong type auto-detected");
    assert(props.find("someint")->type    == PropInt && "Wrong type auto-detected");
    assert(props.find("somevec2i")->type  == PropVec2i && "Wrong type auto-detected");
    assert(props.find("somestring")->type == PropString && "Wrong type auto-detected");
    assert(props.find("someobject")->type == PropInt && "Wrong type auto-detected");

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
    props.registerProperty("someobject", someobject.x, SomeClass::setXNice, &someobject);
    props.set<int>("someobject", 10);
    assert(*props.getAs<int>("someobject") == 10 && "Wrong value");
    assert(someobject.x == someobject.y && "Wrong value");


    // Test serialization
    Json::Value val;
    props.writeToJson(val);
    cout<<val.toStyledString()<<endl;

    somefloat = 0;
    someint = 0;
    somevec2i.fill(0);
    somestring = "";

    assert(*props.getAs<float>("somefloat")       == 0.f && "Wrong value");
    assert(*props.getAs<int>("someint")           == 0 && "Wrong value");
    assert(*props.getAs<math::Vec2i>("somevec2i") == math::Vec2i() && "Wrong value");
    assert(*props.getAs<string>("somestring")     == "" && "Wrong value");

    props.loadFromJson(val);

    assert(*props.getAs<float>("somefloat")       == 7.4f && "Wrong value");
    assert(*props.getAs<int>("someint")           == 42 && "Wrong value");
    assert(*props.getAs<math::Vec2i>("somevec2i") == math::Vec2i(8, 8) && "Wrong value");
    assert(*props.getAs<string>("somestring")     == "test" && "Wrong value");


    props.clear();

    assert(props.size() == 0 && "Wrong size");

    return 0;
}
