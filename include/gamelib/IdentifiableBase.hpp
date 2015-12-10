#ifndef GAMELIB_INTERFACE_IDENTIFIABLE_HPP
#define GAMELIB_INTERFACE_IDENTIFIABLE_HPP

// Base class for objects with ID.
// It only defines a virtual method to get the object's ID.
// Consider inheriting from Identifiable instead (see Identifiable.hpp).

namespace gamelib
{
    typedef unsigned int ID;

    constexpr ID invalidID = -1;

    class IdentifiableBase
    {
        public:
            virtual ~IdentifiableBase() {};
            virtual ID getID() const = 0;
    };
}

#endif
