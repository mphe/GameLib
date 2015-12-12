#include "gamelib/event/CallbackHandler.hpp"
#include <cassert>

using namespace gamelib;

typedef CallbackHandler<int, int, int> TestCBHandler;

// Yes, globals are bad and I should feel bad, but this is just a test so fuck it.
TestCBHandler cbhandler;

int testcallback(void* me, int a, int b);
int testcallback2(void* me, int a, int b);

class Foo
{
    public:
        static int testcallback_class(void* me, int a, int b)
        {
            Foo* me_ = static_cast<Foo*>(me);
            me_->cbhandler->unregCallback(testcallback2, me);
            return a + b;
        }

    public:
        TestCBHandler* cbhandler;
};

int main(int argc, char *argv[])
{
    Foo foo;
    foo.cbhandler = &cbhandler;

    cbhandler.regCallback(testcallback, NULL);
    cbhandler.regCallback(testcallback2, &foo); // deletes Foo::testcallback_class
    cbhandler.regCallback(testcallback, NULL);
    cbhandler.regCallback(Foo::testcallback_class, &foo); // deletes testcallback2
    cbhandler.regCallback(testcallback, NULL);

    assert("Wrong size" && cbhandler.size() == 5);

    cbhandler.call(1, 2);
    cbhandler.clean();

    assert("Wrong size" && cbhandler.size() == 3);


    return 0;
}

int testcallback(void* me, int a, int b)
{
    return a + b;
}

int testcallback2(void* me, int a, int b)
{
    // a pointer to a Foo object will be passed as "me"
    cbhandler.unregCallback(Foo::testcallback_class, me);
    return a + b;
}

