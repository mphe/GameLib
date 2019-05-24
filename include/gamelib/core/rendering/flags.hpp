#ifndef GAMELIB_RENDERING_FLAGS_HPP
#define GAMELIB_RENDERING_FLAGS_HPP

#include "gamelib/utils/bitflags.hpp"
#include "gamelib/utils/utils.hpp"

namespace gamelib
{
    constexpr unsigned int render_invisible     = 1;       // don't render this entity
    constexpr unsigned int render_noparallax    = 1 << 1;  // disable parallax for this entity
    constexpr unsigned int render_hidden        = 1 << 2;  // same as invisible but can be toggled using render_drawhidden
    constexpr unsigned int render_drawhidden    = 1 << 3;  // render render_hidden entities
    constexpr unsigned int render_scaleparallax = 1 << 4;  // Scale objects by their parallax values to create a 3D effect

    constexpr const char* str_renderflags[] = {
        "Invisible",
        "No parallax",
        "Hidden",
        "Draw hidden",
        "Scale parallax",
    };

    constexpr unsigned int num_renderflags = ARRAY_SIZE(str_renderflags);
}

#endif
