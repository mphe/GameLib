#ifndef GAMELIB_LINE_BRUSH_COMPONENT_HPP
#define GAMELIB_LINE_BRUSH_COMPONENT_HPP

#include "BrushComponent.hpp"

namespace gamelib
{
    class LineBrushComponent : public BrushComponent
    {
        public:
            constexpr static const char* name = "LineBrushComponent";

        public:
            LineBrushComponent();

            auto setWidth(int width) -> void;
            auto getWidth() const    -> int;

            auto getBrushPolygon() const -> Polygon*;

            auto regenerate() const -> void;

        private:
            virtual auto _refresh()  -> void;

        private:
            Polygon* _line;
            int _linewidth;
    };
}

#endif
