#ifndef EDITOR_BRUSH_TOOL_HPP
#define EDITOR_BRUSH_TOOL_HPP

#include "Tool.hpp"
#include "gamelib/core/res/TextureResource.hpp"
#include "gamelib/core/ecs/ecsmeta.hpp"
#include "math/geometry/Point2.hpp"

namespace gamelib
{
    class Entity;
    class PolygonBrushComponent;

    class BrushTool : public Tool
    {
        public:
            enum BrushType {
                Polygon,
                Line,
                Rect,
                NumBrushTypes
            };

        public:
            BrushTool();

            void onMousePressed() final override;
            void drawGui() final override;
            void render(sf::RenderTarget& target) final override;

        private:
            auto _getIfSame() const -> ComponentReference<PolygonBrushComponent>;
            auto _apply(ComponentReference<PolygonBrushComponent> brush) const -> void;

        private:
            bool _showdraggers;
            bool _snappoint;
            bool _solid;
            math::Vec2f _offset;
            int _linewidth;

        private:
            BrushType _type;
            TextureResource::Handle _tex;
    };
}

#endif
