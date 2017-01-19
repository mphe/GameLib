#include <cassert>
#include "gamelib/res/ResourceManager.hpp"
#include "gamelib/utils/string.hpp"

using namespace gamelib;

ResourcePtr testLoader(const std::string& fname, ResourceManager* resmgr);
ResourcePtr barLoader(const std::string& fname, ResourceManager* resmgr);

typedef Resource<int, 0x312601c1> TestResource;

int main(int argc, char *argv[])
{
    ResourceManager mgr(nullptr, "assets");
    mgr.registerFileType("test", testLoader);
    mgr.registerFileType("bar", barLoader);

    auto res = mgr.load("foo.test");
    assert(res->as<TestResource>().res == 42 && "Wrong data");
    assert(mgr.find("foo.test") && "Resource should be loaded");

    auto res2 = mgr.get("foo.bar");
    assert(res2->as<TestResource>().res == 13 && "Wrong data");

    auto res3 = mgr.get("foo.bar");
    assert(res3.get() == res2.get() && "Objects should be the same");

    Json::Value val;
    mgr.writeToJson(val);
    std::cout<<val.toStyledString()<<std::endl;

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

ResourcePtr testLoader(const std::string& fname, ResourceManager* resmgr)
{
    std::string testpath = "assets/foo.test";
    adaptPath(testpath);

    assert(fname == testpath && "Wrong path");
    return TestResource::create(42);
}

ResourcePtr barLoader(const std::string& fname, ResourceManager* resmgr)
{
    std::string testpath = "assets/foo.bar";
    adaptPath(testpath);

    assert(fname == testpath && "Wrong path");
        return TestResource::create(13);
}
