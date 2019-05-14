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
#include "gamelib/imgui/resources.hpp"
#include "gamelib/editor/EditorShared.hpp"

namespace gamelib
{
    static const char* brushEntities[] = {
        "brush_polygon",
        "brush_line",
        "brush_rect",
    };

    static const char* brushNames[] = {
        "Polygon",
        "Line",
        "Rectangle",
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
        _linewidth = _tex->getSize().y;
    }

    void BrushTool::onMousePressed()
    {
        auto selected = _getIfSame();
        auto& snapped = EditorShared::getMouseSnapped();

        if (selected)
        {
            auto& pol = selected->getBrushPolygon()->getGlobal();
            math::Point2f p = _snappoint ? snap(pol, EditorShared::getMouse()) : snapped;

            if (_type == Rect && pol.size() == 1)
            {
                auto start = pol.get(0);
                selected->add(math::Point2f(start.x, p.y), false);
                selected->add(math::Point2f(p.x, start.y), false);
            }
            selected->add(p, false);
        }
        else
        {
            auto brush = getEntity(createEntity(brushEntities[_type], snapped.x, snapped.y));
            _apply(brush->findByType<PolygonBrushComponent>());
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
            auto& pol = selected->getBrushPolygon()->getGlobal();

            if (!input->isMouseConsumed())
            {
                if (_snappoint)
                    mouse = snap(pol, EditorShared::getMouse());

                if (_type == Rect && pol.size() == 1)
                {
                    drawRectOutline(target, math::AABBf(pol.get(0), mouse.asVector() - pol.get(0).asVector()), sf::Color::White);
                }
                else if (_type == Polygon && pol.size() > 1)
                {
                    drawLine(target, pol.get(pol.size() - 1), mouse);
                    drawLine(target, pol.get(pol.size() - 2), mouse);
                }
                else if (pol.size() > 0)
                {
                    drawLine(target, pol.get(pol.size() - 1), mouse);
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

        if (inputResource("Texture", &_tex) && _tex)
            _linewidth = _tex->getSize().y;

        ImGui::InputFloat2("Texture offset", &_offset[0], 2);

        ImGui::Checkbox("Solid", &_solid);
        ImGui::Checkbox("Snap to points", &_snappoint);
        ImGui::Checkbox("Show drag boxes", &_showdraggers);

        ImGui::PopItemWidth();

        auto brush = getIfBrush(EditorShared::getSelected());
        if (brush && ImGui::Button("Apply"))
            _apply(brush);
    }

    void BrushTool::_apply(PolygonBrushComponent* brush) const
    {
        if (brush->getEntity()->getName() == brushEntities[Line])
            static_cast<LineBrushComponent*>(brush)->setWidth(_linewidth);

        brush->getBrushShape()->setTexture(_tex);
        brush->getBrushShape()->setTexOffset(_offset);

        if (_solid)
            ADDFLAG(brush->getBrushPolygon()->flags, collision_solid);
        else
            RMFLAG(brush->getBrushPolygon()->flags, collision_solid);
    }

    PolygonBrushComponent* BrushTool::_getIfSame() const
    {
        auto selected = getIfBrush(EditorShared::getSelected());
        if (selected && brushEntities[_type] == selected->getEntity()->getName())
        {
            if (_type == Rect && selected->getBrushPolygon()->size() > 1)
                return nullptr;
            else
                return selected;
        }
        return nullptr;
    }
}
