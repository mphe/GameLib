#include "gamelib/editor/tools/VertexTool.hpp"
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/components/editor/BrushComponent.hpp"
#include "gamelib/components/geometry/Polygon.hpp"

namespace gamelib
{
    VertexTool::VertexTool() :
        _selected(-1),
        _snappoint(true)
    { }

    void VertexTool::onMousePressed()
    {
        static const math::AABBf drag_rect(-drag_size, -drag_size, drag_size * 2, drag_size * 2);

        auto brush = getIfBrush(EditorShared::getSelectTool().getSelected());
        if (brush)
        {
            auto& pol = *brush->getBrushPolygon();
            _selected = -1;

            for (size_t i = 0; i < pol.size(); ++i)
            {
                if (drag_rect.contains(EditorShared::getMouse() - pol.getPolygon().get(i).asVector()))
                {
                    _selected = i;
                    break;
                }
            }
        }

        if (_selected == (size_t)-1)
            EditorShared::getSelectTool().onMousePressed();
    }

    void VertexTool::onDrag()
    {
        auto brush = getIfBrush(EditorShared::getSelectTool().getSelected());

        if (brush && _selected != (size_t)-1)
        {
            if (_snappoint)
                brush->edit(_selected, snap(brush->getBrushPolygon()->getPolygon(), EditorShared::getMouse(), _selected));
            else
                brush->edit(_selected, EditorShared::getMouseSnapped());
        }
    }

    void VertexTool::drawGui()
    {
        ImGui::Checkbox("Snap to points", &_snappoint);
    }

    void VertexTool::render(sf::RenderTarget& target)
    {
        auto brush = getIfBrush(EditorShared::getSelectTool().getSelected());
        if (brush)
            drawDragBoxes(target, brush->getBrushPolygon()->getPolygon(), _selected);
    }
}
