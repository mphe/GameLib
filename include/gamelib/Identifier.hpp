#ifndef GAMELIB_IDENTIFIER_HPP
#define GAMELIB_IDENTIFIER_HPP

#include "Identifiable.hpp"

// A wrapper around Identifiable, used to avoid implementing the same code everytime again by hand.
// It takes an ID as template parameter and provides a getID() method, which returns the given ID.
// Additionally a static (constexpr) variable "id" is set to allow static access to the class's ID.
//
// To avoid possible diamond problems, another template argument is available to specify which class
// "Identifier" should inherit from (Default is Identifiable).
// E.g.:
// class FooInterface : public Identifiable {};
// class FooDerived : public FooInterface, Identifier<0xdeadbeef> {};
//
// This would lead to errors because FooDerived inherits FooInterface twice.
// To avoid this, pass FooInterface as template argument:
//
// class FooInterface : public Identifiable {};
// class FooDerived : Identifier<0xdeadbeef, FooInterface> {};

namespace gamelib
{
    template <ID idval, class T = Identifiable>
    class Identifier : public T
    {
        public:
            static constexpr ID id = idval;

        public:
            virtual ~Identifier() {}

            inline ID getID() const
            {
                return id;
            }
    };
}

#endif
