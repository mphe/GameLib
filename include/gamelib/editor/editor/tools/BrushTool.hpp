#ifndef EDITOR_BRUSH_TOOL_HPP
#define EDITOR_BRUSH_TOOL_HPP

#include "Tool.hpp"
#include "gamelib/core/res/TextureResource.hpp"
#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class Entity;
    class BrushComponent;

    class BrushTool : public Tool
    {
        public:
            enum BrushType {
                Polygon,
                Line,
                NumBrushTypes
            };

        public:
            BrushTool();

            void onMousePressed();
            void drawGui();
            void render(sf::RenderTarget& target);

        private:
            BrushComponent* _getIfSame() const;

        private:
            bool _showdraggers;
            bool _snappoint;
            math::Vec2f _offset;
            int _linewidth;

        private:
            BrushType _type;
            TextureResource::Handle _tex;
    };
}

#endif
