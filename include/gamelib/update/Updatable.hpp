#ifndef GAMELIB_UPDATABLE_HPP
#define GAMELIB_UPDATABLE_HPP

#include "gamelib/Identifier.hpp"

namespace gamelib
{
    class Updatable
    {
        public:
            Updatable(int interval = 0);
            virtual ~Updatable() {}

            virtual void update(float elapsed) = 0;

        public:
            int interval; // Used by UpdateSystem
    };
}

#endif
