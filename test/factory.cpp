#include "gamelib/utils/Factory.hpp"
#include <string>
#include <cassert>

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

        const string& getName() const final override { return name; }

        static std::unique_ptr<Foo> create(const string& name)
        {
            return std::unique_ptr<Foo>(new Bar(name));
        }

        static std::unique_ptr<Foo> create()
        {
            return std::unique_ptr<Foo>(new Bar());
        }

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
    gamelib::Factory<Foo, int> fac;
    fac.add(42, Bar::create);
    fac.add<Bla>(75);
    assert(fac.size() == 2);

    auto ptr1 = fac.create(42);
    auto ptr2 = fac.create(75);

    assert(ptr1->getName() == "unnamed Bar");
    assert(ptr2->getName() == "unnamed Bla");

    fac.remove(42);
    assert(fac.size() == 1);

    fac.clear();
    assert(fac.size() == 0);

    gamelib::Factory<Foo, int, const string&> argfac;
    argfac.add(42, Bar::create);
    argfac.add<Bla>(75);

    auto ptr3 = argfac.create(42, "argfac Bar");
    auto ptr4 = argfac.create(75, "argfac Bla");

    assert(ptr3->getName() == "argfac Bar");
    assert(ptr4->getName() == "argfac Bla");

    return 0;
}
