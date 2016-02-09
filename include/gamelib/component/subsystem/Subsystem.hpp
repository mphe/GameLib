#ifndef GAMELIB_SUBSYSTEM_HPP
#define GAMELIB_SUBSYSTEM_HPP

#include <stddef.h> // size_t
#include "gamelib/Identifiable.hpp"

namespace gamelib
{
    class Subsystem : public Identifiable
    {
        public:
            virtual ~Subsystem() {};

            virtual void update(float fps) = 0;
            virtual void destroy() = 0;
            virtual size_t size() const = 0;
    };
}

#endif
