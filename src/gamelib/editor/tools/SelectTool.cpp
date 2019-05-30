#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/components/CollisionComponent.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/rendering/RenderSystem.hpp"
#include "gamelib/core/geometry/CollisionSystem.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/events/OnSelect.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "imgui.h"

namespace gamelib
{
    SelectTool::SelectTool() :
        renderBBox(true),
        renderAllBoxes(false),
        _scaleselect(-1),
        _mode(mode_move),
        _cloned(false),
        _switchmode(false)
    { }

    void SelectTool::onMouseRelease()
    {
        _scaleselect = -1;
        _cloned = false;

        if (_switchmode)
        {
            _mode = (_mode + 1) % num_modes;
            _switchmode = false;
        }
    }

    void SelectTool::onMousePressed()
    {
        if (_mode == mode_scale)
        {
            auto& box = getSelected()->getTransform().getBBox();
            size_t i = 0;
            for (float x : { 0.f, 0.5f, 1.f })
                for (float y : { 0.f, 0.5f, 1.f })
                    if (x != 0.5f || y != 0.5f)
                    {
                        if (checkDragBox(box.pos.asPoint() + math::Vec2f(x, y) * box.size))
                        {
                            _scaleselect = i;
                            return;
                        }
                        ++i;
                    }
        }

        auto old = getSelected();
        select(EditorShared::getMouse().x, EditorShared::getMouse().y);

        auto ent = getSelected();
        if (ent)
        {
            _dragoffset = EditorShared::getMouse() - ent->getTransform().getPosition();

            if (old == ent)
                _switchmode = true;
        }
        else    // deselected
            _mode = mode_move;
    }

    void SelectTool::onDrag()
    {
        auto* ent = getSelected();
        if (!ent)
            return;

        auto input = getSubsystem<InputSystem>();
        auto& bbox = ent->getTransform().getBBox();

        // Don't switch to next mode if something was dragged
        _switchmode = false;

        if (_mode == mode_scale && _scaleselect != (size_t)-1)
        {
            math::Point2f start = bbox.pos.asPoint(),
                          end = start + bbox.size;

            auto newp = EditorShared::getMouseSnapped();

            // corners: 0 2 5 7
            // edges: h: 1 6  v: 3 4
            switch (_scaleselect)
            {
                case 0:
                    start = newp;
                    break;
                case 1:
                    start.x = newp.x;
                    break;
                case 2:
                    start.x = newp.x;
                    end.y = newp.y;
                    break;
                case 3:
                    start.y = newp.y;
                    break;
                case 4:
                    end.y = newp.y;
                    break;
                case 5:
                    end.x = newp.x;
                    start.y = newp.y;
                    break;
                case 6:
                    end.x = newp.x;
                    break;
                case 7:
                    end = newp;
                    break;
            }

            auto scalebox = math::AABBf::fromPoints(start, end);
            if (math::almostEquals(scalebox.w, 0.f))
                scalebox.w = 0.01;
            if (math::almostEquals(scalebox.h, 0.f))
                scalebox.h = 0.01;
            auto scale = scalebox.size / bbox.size;

            // TODO: maybe support flipping

            // It's important to get the new bbox, as it might have changed during scaling
            // because the origin is not necessarily at the top left.
            ent->getTransform().scale(scale);
            ent->getTransform().move(scalebox.pos - ent->getTransform().getBBox().pos);
            return;
        }

        if (!_cloned && input->isKeyDown(sf::Keyboard::LShift))
        {
            Json::Value cfg;
            writeToJson(cfg, *ent);
            ent = getEntity(getSubsystem<EntityManager>()->add());
            if (!loadFromJson(cfg, *ent))
                LOG_ERROR("Failed to clone entity");
            select(ent);
            _cloned = true;
        }

        ent->getTransform().setPosition(
                EditorShared::snap(EditorShared::getMouse() - _dragoffset));
    }

    void SelectTool::render(sf::RenderTarget& target)
    {
        auto* ent = getSelected();
        if (!ent)
            return;

        if (renderBBox)
        {
            auto box = ent->getTransform().getBBox();
            if (renderAllBoxes)
                box.extend(math::AABBf(0, 0, 0.5, 0.5));
            drawRectOutline(target, box);
        }

        if (_mode == mode_scale)
        {
            auto& bbox = ent->getTransform().getBBox();
            size_t i = 0;
            for (float x : { 0.f, 0.5f, 1.f })
                for (float y : { 0.f, 0.5f, 1.f })
                    if (x != 0.5f || y != 0.5f)
                        drawDragBox(target, bbox.pos.asPoint() + math::Vec2f(x, y) * bbox.size, _scaleselect == (i++));
        }

        if (renderAllBoxes)
            for (auto& i : ent->getTransform().getChildren())
                drawRectOutline(target, i->getBBox(), sf::Color::Green);

        auto& pos = ent->getTransform().getPosition();
        sf::Vertex cross[4];
        cross[0] = sf::Vertex(sf::Vector2f(pos.x - 2, pos.y - 2), sf::Color::Red);
        cross[1] = sf::Vertex(sf::Vector2f(pos.x + 2, pos.y + 2), sf::Color::Red);
        cross[2] = sf::Vertex(sf::Vector2f(pos.x + 2, pos.y - 2), sf::Color::Red);
        cross[3] = sf::Vertex(sf::Vector2f(pos.x - 2, pos.y + 2), sf::Color::Red);
        target.draw(cross, 4, sf::Lines);
    }

    void SelectTool::drawGui()
    {
        ImGui::Checkbox("Show bounding box", &renderBBox);
        ImGui::Checkbox("Show child bounding boxes", &renderAllBoxes);
        ImGui::Text("Mode: %i", _mode);
    }

    void SelectTool::select(Entity::Handle enthandle)
    {
        if (enthandle == _selected)
            return;
        else
            _mode = mode_move;

        auto old = _selected;

        if (getEntity(enthandle))
        {
            _selected = enthandle;
            LOG("Selected entity ", enthandle.index, enthandle.version);
        }
        else
        {
            _selected = Entity::Handle();
            LOG("Selection cleared");
        }

        EventManager::getActive()->triggerEvent(OnSelectEvent::create(old, _selected));
    }

    void SelectTool::select(Entity* ent)
    {
        if (ent)
            select(ent->getHandle());
        else
            select(Entity::Handle());
    }

    void SelectTool::select(float x, float y)
    {
        auto rensys = getSubsystem<RenderSystem>();
        if (!rensys)
        {
            select(nullptr);
            return;
        }

        auto node = rensys->getNode(
                rensys->getNodeAtPosition(math::Point2f(x, y)));

        if (node && node->owner)
            select(node->owner->getEntity());
        else
            select(nullptr);
    }

    const Entity* SelectTool::getSelected() const
    {
        return getEntity(_selected);
    }

    Entity* SelectTool::getSelected()
    {
        return getEntity(_selected);
    }
}
