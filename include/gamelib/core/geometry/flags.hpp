#ifndef GAMELIB_COLLISION_FLAGS_HPP
#define GAMELIB_COLLISION_FLAGS_HPP

#include "gamelib/utils/bitflags.hpp"

namespace gamelib
{
    constexpr unsigned int collision_solid     = 1;
    constexpr unsigned int collision_mask      = 1 << 1;
    constexpr unsigned int collision_noprecise = 1 << 2;
}

#endif
