#ifndef GAMELIB_LINE_BRUSH_COMPONENT_HPP
#define GAMELIB_LINE_BRUSH_COMPONENT_HPP

#include "PolygonBrushComponent.hpp"

namespace gamelib
{
    class LineBrushComponent : public PolygonBrushComponent
    {
        public:
            ASSIGN_NAMETAG("LineBrushComponent");

        public:
            LineBrushComponent();

            auto setWidth(int width) -> void;
            auto getWidth() const    -> int;

            auto getBrushPolygon() const -> PolygonCollider* final override;

            auto regenerate() const -> void final override;

        private:
            ComponentReference<PolygonCollider> _line;
            int _linewidth;
    };
}

#endif
