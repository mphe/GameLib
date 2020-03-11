#include <cassert>
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/utils/string.hpp"
#include "gamelib/utils/utils.hpp"

using namespace gamelib;

typedef Resource<int, 0x312601c1> TestResource;

template <int testval>
BaseResourceHandle testLoader(const std::string& fname, UNUSED ResourceManager* resmgr)
{
    LOG("fname ", fname);
    return TestResource::create(testval).as<BaseResource>();
}


auto testload(const std::string& fname, int expectedval, ResourceManager& resmgr) -> TestResource::Handle
{
    auto res = resmgr.get(fname).as<TestResource>();
    assert(*res == expectedval && "Wrong data");
    assert(resmgr.find(fname) && "Resource should be loaded");
    assert(resmgr.find(res.getResource()->getFullPath()) && "Resource should contain correct load path");
    return res;
}

template <typename T>
auto compare(const T& a, const T& b, bool equal) -> bool
{
    return equal ? a == b : a != b;
}

auto testequality(BaseResourceHandle a, BaseResourceHandle b, bool equal = true) -> void
{
    assert(compare(b.getResource(),  a.getResource(), equal) && "Objects should be the (not) same");
    assert(compare(b, a, equal) && "Objects should be the (not) same");
    assert(!compare(b, a, !equal) && "Objects should be the (not) same");
}

int main()
{
    ResourceManager mgr;

    assert(mgr.addSearchpath("testassets") && "Searchpath should exist");
    assert(mgr.addSearchpath("testassets2") && "Searchpath should exist");

    mgr.registerFileType("test", testLoader<42>);
    mgr.registerFileType("bar", testLoader<13>);
    mgr.registerFileType("cpp", testLoader<5>);

    // test relative load
    auto cpp = testload("resmgr.cpp", 5, mgr);

    // test searchpath load
    auto footest = testload("foo.test", 42, mgr);
    auto foobar = testload("foo.bar", 13, mgr);

    // test shadowing
    assert(foobar.getResource()->getSearchpath().find("testassets2") != std::string::npos && "Should be loaded from testassets2");

    // test equality
    auto foobar2 = mgr.get("foo.bar");
    testequality(foobar, foobar2);
    foobar.reset();
    testequality(foobar, foobar2, false);
    foobar = foobar2;
    testequality(foobar, foobar2);

    // test absolute load + shadow bypass + equality
    auto path = boost::filesystem::canonical("testassets/foo.bar");
    auto shadowedfoobar = testload(path.string(), 13, mgr);
    testequality(shadowedfoobar, foobar, false);
    assert(!shadowedfoobar.getResource()->getSearchpath().empty() && "Should be found in searchpath");
    assert(shadowedfoobar.getResource()->getSearchpath().find("testassets2") == std::string::npos && "Should not be loaded from testassets2");

    // test write serialization
    Json::Value val;
    mgr.writeToJson(val);
    LOG(val.toStyledString());

    footest.reset();
    mgr.clean();

    assert(!mgr.find("foo.test") && "Resource should not exist");
    assert(mgr.loadFromJson(val) && "Failed to load from json");
    assert(mgr.find("foo.test") && "Resource should be loaded");

    mgr.clear();

    // test load serialization
    assert(mgr.loadFromJson(val) && "Failed to load from json");
    assert(mgr.find("foo.test") && "Resource should be loaded");
    assert(mgr.find("foo.bar") && "Resource should be loaded");

    return 0;
}
