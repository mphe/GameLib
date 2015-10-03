#include "utils/Factory.hpp"
#include <string>
#include <cassert>

using namespace factory;
using namespace std;

class Foo
{
    public:
        virtual ~Foo() {}
        virtual const string& getName() const = 0;
};

class Bar : public Foo
{
    public:
        Bar() : name("unnamed Bar") {}
        Bar(const string& name_) : name(name_) {}
        virtual ~Bar() {}

        const string& getName() const { return name; }

        static Foo* create(const string& name) { return new Bar(name); }
        static Foo* create() { return new Bar(); }

    private:
        string name;
};

class Bla : public Bar
{
    public:
        Bla() : Bar("unnamed Bla") {}
        Bla(const string& name) : Bar(name) {}
};

int main()
{
    Factory<Foo, int> fac;
    fac.add(42, Bar::create);
    fac.add<Bla>(75);
    assert(fac.size() == 2);

    Foo *ptr1, *ptr2;
    ptr1 = fac.create(42);
    ptr2 = fac.create(75);

    assert(ptr1->getName() == "unnamed Bar");
    assert(ptr2->getName() == "unnamed Bla");

    delete ptr1;
    delete ptr2;

    fac.del(42);
    assert(fac.size() == 1);

    fac.clear();
    assert(fac.size() == 0);

    Factory<Foo, int, const string&> argfac;
    argfac.add(42, Bar::create);
    argfac.add<Bla>(75);

    ptr1 = argfac.create(42, "argfac Bar");
    ptr2 = argfac.create(75, "argfac Bla");

    assert(ptr1->getName() == "argfac Bar");
    assert(ptr2->getName() == "argfac Bla");

    delete ptr1;
    delete ptr2;

    return 0;
}
