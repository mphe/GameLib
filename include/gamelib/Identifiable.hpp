#ifndef GAMELIB_IDENTIFIABLE_HPP
#define GAMELIB_IDENTIFIABLE_HPP

#include "IdentifiableBase.hpp"

// A wrapper around IdentifiableBase.
// It takes an ID as template parameter and provides a getID() method, which returns the given ID.
// Additionally a static (constexpr) variable "id" is set to allow static access to the class's ID.

namespace gamelib
{
    template <ID ID_>
    class Identifiable : virtual public IdentifiableBase
    {
        public:
            static constexpr ID id = ID_;

        public:
            virtual ~Identifiable() {}

            inline ID getID() const
            {
                return id;
            }
    };
}

#endif
