#include "gamelib/editor/tools/VertexTool.hpp"
#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "gamelib/core/ecs/Entity.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/components/editor/PolygonBrushComponent.hpp"
#include "gamelib/components/geometry/Polygon.hpp"

namespace gamelib
{
    VertexTool::VertexTool() :
        _selected(-1),
        _snappoint(true)
    { }

    void VertexTool::onMousePressed()
    {
        auto brush = getIfBrush(EditorShared::getSelectTool().getSelected());
        if (brush)
            selectDragBoxes(brush->getBrushPolygon()->getGlobal(), &_selected);

        if (_selected == (size_t)-1)
            EditorShared::getSelectTool().select(EditorShared::getMouse().x, EditorShared::getMouse().y);
    }

    void VertexTool::onDrag()
    {
        auto brush = getIfBrush(EditorShared::getSelectTool().getSelected());

        if (brush && _selected != (size_t)-1)
        {
            if (_snappoint)
                brush->edit(_selected, snap(brush->getBrushPolygon()->getGlobal(), EditorShared::getMouse(), _selected), false);
            else
                brush->edit(_selected, EditorShared::getMouseSnapped(), false);
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
            drawDragBoxes(target, brush->getBrushPolygon()->getGlobal(), _selected);
    }
}
