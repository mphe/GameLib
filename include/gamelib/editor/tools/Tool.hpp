#ifndef EDITOR_TOOL_HPP
#define EDITOR_TOOL_HPP

#include <SFML/Graphics/RenderTarget.hpp>
#include "gamelib/core/rendering/Renderable.hpp"
#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class Tool : public Renderable
    {
        public:
            virtual ~Tool() {}

            virtual void onMousePressed() {};
            virtual void onMouseRelease() {};
            virtual void onMouseMove() {};
            virtual void onDrag() {};
            virtual void drawGui() {};
            virtual void render(sf::RenderTarget& target) {};
    };
}

#endif
