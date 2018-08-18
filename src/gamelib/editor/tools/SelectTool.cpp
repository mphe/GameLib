#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/core/event/EventManager.hpp"
#include "gamelib/core/ecs/RenderComponent.hpp"
#include "gamelib/core/ecs/CollisionComponent.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/geometry/CollisionSystem.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/events/OnSelect.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "imgui.h"

namespace gamelib
{
    SelectTool::SelectTool() :
        renderBBox(true),
        renderAllBoxes(false)
    { }

    void SelectTool::onMousePressed()
    {
        select(EditorShared::getMouse().x, EditorShared::getMouse().y, 0);

        auto* ent = getSelected();
        if (ent)
            _dragoffset = EditorShared::getMouse() - ent->getTransform().getPosition();
    }

    void SelectTool::onDrag()
    {
        auto* ent = getSelected();
        if (ent)
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
    }

    void SelectTool::select(Entity::Handle enthandle)
    {
        if (enthandle == _selected)
            return;

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

    void SelectTool::select(float x, float y, unsigned int flags)
    {
        Entity* top = nullptr;
        int depth = 0;
        int layerdepth = 0;

        auto selector = [&](Collidable* col) {
            auto ent = static_cast<CollisionComponent*>(col)->getEntity();
            auto ren = ent->findByType<RenderComponent>();
            if (!ren || !ren->isVisible())
                return false;

            auto layer = Scene::getActive()->getLayer(ren->getLayer());
            int layerd = layer ? layer->getDepth() : 0;
            int d = ren->getDepth();
            if (top == nullptr || layerd < layerdepth || (layerd == layerdepth && d < depth))
            {
                depth = d;
                layerdepth = layerd;
                top = ent;
            }
            return false;
        };

        CollisionSystem::getActive()->intersectAll(math::Point2f(x, y), nullptr, flags, selector);
        select(top);
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
