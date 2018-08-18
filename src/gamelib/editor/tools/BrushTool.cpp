#include "gamelib/editor/tools/BrushTool.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "gamelib/utils/log.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/components/editor/LineBrushComponent.hpp"
#include "gamelib/components/geometry/Polygon.hpp"
#include "gamelib/components/rendering/PolygonShape.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/ui/resources.hpp"
#include "gamelib/editor/EditorShared.hpp"

namespace gamelib
{
    static const char* brushEntities[] = {
        "brush_polygon",
        "brush_line",
    };

    static const char* brushNames[] = {
        "Polygon",
        "Line",
    };


    BrushTool::BrushTool() :
        _showdraggers(true),
        _snappoint(true),
        _solid(false),
        _linewidth(32),
        _type(Polygon),
        _tex(nullptr)
    {
        _tex = getSubsystem<ResourceManager>()->find<TextureResource>();
    }

    void BrushTool::onMousePressed()
    {
        auto selected = _getIfSame();

        if (selected)
        {
            math::Point2f p;

            if (_snappoint)
                p = snap(selected->getBrushPolygon()->getPolygon(), EditorShared::getMouse());
            else
                p = EditorShared::getMouseSnapped();

            LOG_DEBUG("Creating Vertex at (", p.x, ", ", p.y, ")");
            selected->add(p);
        }
        else
        {
            auto brush = getEntity(createEntity(brushEntities[_type]));
            _apply(brush->findByType<BrushComponent>());
            brush->getTransform().setPosition(EditorShared::getMouseSnapped());
            EditorShared::getSelectTool().select(brush);
            onMousePressed();
        }
    }

    void BrushTool::render(sf::RenderTarget& target)
    {
        math::Point2f mouse = EditorShared::getMouseSnapped();
        auto selected = _getIfSame();
        auto input = getSubsystem<InputSystem>();

        if (selected)
        {
            auto& pol = selected->getBrushPolygon()->getPolygon();

            if (!input->isMouseConsumed())
            {
                if (_snappoint)
                    mouse = snap(pol, EditorShared::getMouse());

                if (_type == Polygon && pol.size() > 1)
                {
                    drawLine(target, pol.get(-1), mouse);
                    drawLine(target, pol.get(-2), mouse);
                }
                else if (pol.size() > 0)
                {
                    drawLine(target, pol.get(-1), mouse);
                }
            }

            if (_showdraggers)
                drawDragBoxes(target, pol);
        }

        if (_showdraggers && !input->isMouseConsumed())
            drawDragBox(target, mouse);
    }

    void BrushTool::drawGui()
    {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
        ImGui::Combo("Brush type", (int*)&_type, brushNames, NumBrushTypes);

        if (_type == Line)
        {
            ImGui::InputInt("Line width", &_linewidth, 1, 32);
            if (_tex && _linewidth != (int)_tex->getSize().y)
            {
                ImGui::SameLine();
                if (ImGui::Button("Reset"))
                    _linewidth = _tex->getSize().y;
            }
        }

        if (inputResource(&_tex) && _tex)
            _linewidth = _tex->getSize().y;

        ImGui::InputFloat2("Texture offset", &_offset[0], 2);

        ImGui::Checkbox("Solid", &_solid);
        ImGui::Checkbox("Snap to points", &_snappoint);
        ImGui::Checkbox("Show drag boxes", &_showdraggers);

        ImGui::PopItemWidth();

        auto brush = _getIfSame();
        if (brush && ImGui::Button("Apply"))
            _apply(brush);
    }

    void BrushTool::_apply(BrushComponent* brush) const
    {
        if (_type == Line)
            static_cast<LineBrushComponent*>(brush)->setWidth(_linewidth);

        brush->getBrushShape()->texture = _tex;
        brush->getBrushShape()->setTexOffset(_offset);

        if (_solid)
            ADDFLAG(brush->getBrushPolygon()->flags, collision_solid);
        else
            RMFLAG(brush->getBrushPolygon()->flags, collision_solid);
    }

    BrushComponent* BrushTool::_getIfSame() const
    {
        auto selected = getIfBrush(EditorShared::getSelectTool().getSelected());
        bool linebrush = selected && selected->getEntity()->getName() == brushEntities[Line];

        if (((_type == Line) ^ linebrush))
            return nullptr;
        return selected;
    }
}
