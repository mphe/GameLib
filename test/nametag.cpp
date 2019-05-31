#include <cassert>
#include "gamelib/utils/nametag.hpp"

using namespace std;
using namespace gamelib;


struct HasNameTag : public INameTag
{
    ASSIGN_NAMETAG("Foo");
};

struct NoNameableBase
{
    inline static const std::string& name()
    {
        static const std::string name = "asf";
        return name;
    }

    inline const std::string& getName() const
    {
        return name();
    }
};

struct NoTag { };

struct WrongStaticType
{
    inline static int name()
    {
        return 0;
    }
};

struct WrongVirtualType
{
    inline static const std::string& name()
    {
        static const std::string name = "asf";
        return name;
    }

    inline int getName() const
    {
        return 0;
    }
};

#define TEST_CLASS(classname, value)   \
    static_assert(has_nametag<classname>() == value, #classname " should be " #value);

int main()
{
    TEST_CLASS(HasNameTag, 1);
    TEST_CLASS(NoNameableBase, 0);
    TEST_CLASS(NoTag, 0);
    TEST_CLASS(WrongStaticType, 0);
    TEST_CLASS(WrongVirtualType, 0);
    return 0;
}
