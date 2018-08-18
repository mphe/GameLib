#ifndef GAMELIB_OVERLAY_HPP
#define GAMELIB_OVERLAY_HPP

#include <SFML/Graphics.hpp>

namespace gamelib
{
    class Overlay
    {
        public:
            Overlay();

            auto render(sf::RenderTarget& target) -> void;

        public:
            bool renderSolid;
            bool renderNormals;
            bool renderVel;
            bool showCoords;
    };
}

#endif
