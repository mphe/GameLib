#ifndef GAMELIB_OVERLAY_HPP
#define GAMELIB_OVERLAY_HPP

#include <SFML/Graphics.hpp>

namespace gamelib
{
    constexpr const char* overlay_strings[] = {
        "Show solids",
        "Show solid normals",
        "Show velocity",
    };

    class Overlay
    {
        public:
            Overlay();

            auto render(sf::RenderTarget& target) -> void;

        public:
            union {
                bool flags[3];
                struct {
                    bool renderSolid;
                    bool renderNormals;
                    bool renderVel;
                };
            };
    };
}

#endif
