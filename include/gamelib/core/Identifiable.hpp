#ifndef GAMELIB_INTERFACE_IDENTIFIABLE_HPP
#define GAMELIB_INTERFACE_IDENTIFIABLE_HPP

// Base class for objects with an ID.
// It only defines a virtual method to get the object's ID.
// Consider inheriting from Identifier instead (see Identifier.hpp).

namespace gamelib
{
    typedef unsigned int ID;

    constexpr ID invalidID = -1;

    class Identifiable
    {
        public:
            virtual ~Identifiable() {};
            virtual ID getID() const = 0;
    };
}

#endif
