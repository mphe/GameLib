#ifndef GAMELIB_ECS_FLAGS_HPP
#define GAMELIB_ECS_FLAGS_HPP

#include "gamelib/utils/bitflags.hpp"

namespace gamelib
{
    constexpr unsigned int entity_persistent = 1;       // unimplemented
    constexpr unsigned int entity_static     = 1 << 1;  // unimplemented
}

#endif
