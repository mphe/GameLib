#include "gamelib/utils/LifetimeTracker.hpp"
#include <cassert>

using namespace std;
using namespace gamelib;

struct A : public LifetimeTracker<A> { int x; };

struct B : public LifetimeTracker<B> { };

struct C : public A {};

typedef A::LTRef ARef;
typedef B::LTRef BRef;
typedef LifetimeReference<C> CRef;

template <typename T>
void checkHandle(const LifetimeReference<T>& ref, T* ptr, bool expectedValid = true)
{
    if (expectedValid)
    {
        assert(ref && "Reference should be valid");
        assert(ref.get() == ptr && "Reference points to wrong address");
        assert(ref == ptr->getLTReference() && "References should be equal");
    }
    else
    {
        assert(!ref && "Reference should be invalid");
        assert(ref.get() == nullptr && "Reference should be nullptr");
    }
}

int main()
{
    A as[2];
    auto h0original = as[0].getLTReference();
    auto h1original = as[1].getLTReference();

    checkHandle(h0original, &as[0]);
    checkHandle(h1original, &as[1]);

    B bs[2];
    checkHandle(bs[0].getLTReference(), &bs[0]);
    checkHandle(bs[1].getLTReference(), &bs[1]);

    { // Test copy
        as[1] = as[0];
        checkHandle(h1original, &as[1], false);
        checkHandle(as[1].getLTReference(), &as[1]);

        as[0] = A();
        checkHandle(h0original, &as[0], false);
        checkHandle(as[0].getLTReference(), &as[0]);
    }

    A::LTRef a3ref;

    { // Test move
        // Move a0 to a3 -> a0 pointer should get updated to &a3
        auto tmp = as[0].getLTReference();
        A a3(std::move(as[0]));
        a3.x = 5;

        a3ref = a3.getLTReference();
        checkHandle(tmp, &a3);
        checkHandle(a3.getLTReference(), &a3);
        checkHandle(as[0].getLTReference(), &as[0], false); // a0 should now be in an empty state
        assert(as[0].getLTReference() != a3.getLTReference() && "References should not be equal");

        // Test dereference
        tmp->x = 5;
        (*tmp).x = 5;

        // check nullptr assignment
        tmp = nullptr;
        checkHandle(tmp, &a3, false);

        tmp = &a3;
        tmp.reset();
        checkHandle(tmp, &a3, false);
    }

    // a3 should be destroyed now because out of scope
    checkHandle(a3ref, (A*)nullptr, false);

    { // Test cast
        assert((std::is_assignable<ARef, A*>::value) && "Should be assignable");
        assert((!std::is_assignable<A*, ARef>::value) && "Should not be assignable");
#define ASSERT_ASSIGN(lhs, rhs, expected) assert((std::is_assignable<lhs, rhs>::value) == expected && (expected ? "Should be assignable" : "Should not be assignable"));

        ASSERT_ASSIGN(A*, ARef, false);

        ASSERT_ASSIGN(ARef, A*, true);
        ASSERT_ASSIGN(ARef, B*, false);
        ASSERT_ASSIGN(ARef, C*, true);

        ASSERT_ASSIGN(ARef, ARef, true);
        ASSERT_ASSIGN(ARef, BRef, false);
        ASSERT_ASSIGN(ARef, CRef, true);

        ASSERT_ASSIGN(CRef, A*, false);
        ASSERT_ASSIGN(CRef, B*, false);
        ASSERT_ASSIGN(CRef, C*, true);

        ASSERT_ASSIGN(CRef, ARef, false);
        ASSERT_ASSIGN(CRef, BRef, false);
        ASSERT_ASSIGN(CRef, CRef, true);

        A a;
        B b;
        C c;
        ARef aref;
        BRef bref;
        CRef cref;

        aref = cref.as<A>();
        assert(aref.get() == static_cast<A*>(cref.get()) && "Reference points to wrong address");
        cref = aref.as<C>();
        assert(cref.get() == static_cast<C*>(aref.get()) && "Reference points to wrong address");
    }

    // test create from nullptr
    ARef aref(nullptr);
    CRef cref(nullptr);

    return 0;
}
