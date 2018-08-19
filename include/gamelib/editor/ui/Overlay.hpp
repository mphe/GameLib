#ifndef GAMELIB_OVERLAY_HPP
#define GAMELIB_OVERLAY_HPP

#include <SFML/Graphics.hpp>

namespace gamelib
{
    class Overlay
    {
        public:
            Overlay();

            auto drawDebugOverlay() -> void;
            auto drawGui() -> void;
            auto render(sf::RenderTarget& target) -> void;

        public:
            bool renderSolid;
            bool renderNormals;
            bool renderVel;
            bool showCoords;
            bool wireframe;
            bool debugOverlay;
            bool debugOverlayMovable;
    };
}

#endif
