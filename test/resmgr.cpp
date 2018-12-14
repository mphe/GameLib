#include <cassert>
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/string.hpp"

using namespace gamelib;

BaseResourceHandle testLoader(const std::string& fname, ResourceManager* resmgr);
BaseResourceHandle barLoader(const std::string& fname, ResourceManager* resmgr);

typedef Resource<int, 0x312601c1> TestResource;

int main()
{
    ResourceManager mgr("testassets");
    mgr.registerFileType("test", testLoader);
    mgr.registerFileType("bar", barLoader);

    auto res = mgr.load("foo.test");
    assert(*res.as<TestResource>() == 42 && "Wrong data");
    assert(mgr.find("foo.test") && "Resource should be loaded");

    auto res2 = mgr.get("foo.bar");
    assert(*res2.as<TestResource>() == 13 && "Wrong data");

    auto res3 = mgr.get("foo.bar");
    assert(res3.getResource() == res2.getResource() && "Objects should be the same");

    assert(res2 == res3 && "Objects should be the same");
    assert(!(res2 != res3) && "Objects should be the same");

    res2.reset();
    assert(res2 != res3 && "Objects should not be the same");

    Json::Value val;
    mgr.writeToJson(val);
    LOG(val.toStyledString());

    res.reset();
    mgr.clean();

    assert(!mgr.find("foo.test") && "Resource should not exist");
    assert(mgr.loadFromJson(val) && "Failed to load from json");
    assert(mgr.find("foo.test") && "Resource should be loaded");

    mgr.clear();
    // assert(mgr.loadFromJson(val) && "Failed to load from json");
    // assert(mgr.find("foo.test") && "Resource should be loaded");
    // assert(mgr.find("foo.bar") && "Resource should be loaded");

    return 0;
}

BaseResourceHandle testLoader(const std::string& fname, ResourceManager* resmgr)
{
    std::string testpath = "testassets/foo.test";
    adaptPath(&testpath);
    LOG("fname ", fname);
    LOG("testpath ", testpath);

    assert(fname == testpath && "Wrong path");
    return TestResource::create(42).as<BaseResource>();
}

BaseResourceHandle barLoader(const std::string& fname, ResourceManager* resmgr)
{
    std::string testpath = "testassets/foo.bar";
    adaptPath(&testpath);
    LOG("fname ", fname);
    LOG("testpath ", testpath);

    assert(fname == testpath && "Wrong path");
    return TestResource::create(13).as<BaseResource>();
}
