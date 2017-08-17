#ifndef GAMELIB_RENDERING_FLAGS_HPP
#define GAMELIB_RENDERING_FLAGS_HPP

#include "gamelib/utils/bitflags.hpp"

namespace gamelib
{
    constexpr unsigned int render_invisible  = 1;       // don't render this entity
    constexpr unsigned int render_noparallax = 1 << 1;  // disable parallax for this entity
    constexpr unsigned int render_wireframe  = 1 << 2;  // unused
    constexpr unsigned int render_hidden     = 1 << 3;  // same as invisible but can be toggled using render_drawhidden
    constexpr unsigned int render_drawhidden = 1 << 4;  // unused (set to render render_hidden entities)
}

#endif
