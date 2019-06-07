#ifndef GAMELIB_COLLISION_FLAGS_HPP
#define GAMELIB_COLLISION_FLAGS_HPP

#include "gamelib/utils/bitflags.hpp"
#include "gamelib/utils/utils.hpp"

namespace gamelib
{
    constexpr unsigned int collision_solid       = 1;
    // Free slot
    constexpr unsigned int collision_noprecise   = 1 << 2;
    constexpr unsigned int collision_physicshull = 1 << 3;
    constexpr unsigned int collision_physicsdrag = 1 << 4;
    constexpr unsigned int collision_hitbox      = 1 << 5;

    constexpr const char* str_colflags[] = {
        "Solid",
        nullptr,
        "Unprecise",
        "Physics Hull",
        "Physics Drag",
        "Hitbox",
    };

    constexpr unsigned int num_colflags = ARRAY_SIZE(str_colflags);
}

#endif
