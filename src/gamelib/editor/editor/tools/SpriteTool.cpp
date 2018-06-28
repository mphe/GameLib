#include "gamelib/editor/editor/tools/SpriteTool.hpp"
#include "gamelib/editor/editor/tools/SelectTool.hpp"
#include "gamelib/editor/editor/EditorShared.hpp"
#include "gamelib/editor/editor/ui/resources.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/input/InputSystem.hpp"
#include "gamelib/components/rendering/SpriteComponent.hpp"
#include "gamelib/components/geometry/AABB.hpp"
#include "gamelib/utils/conversions.hpp"
#include "imgui.h"

namespace gamelib
{
    void SpriteTool::onMousePressed()
    {
        if (_sprite)
        {
            auto& pos = EditorShared::getMouseSnapped();
            auto handle = createEntity("brush_sprite", pos.x, pos.y);
            auto sprite = getEntity(handle)->findByName<SpriteComponent>();
            sprite->change(_sprite);
            auto col = getEntity(handle)->findByName<AABB>();
            auto size = sprite->getLocalBounds().size;
            col->setSize(size);
            col->move(-size / 2);
            EditorShared::getSelectTool().select(handle);
        }
    }

    void SpriteTool::drawGui()
    {
        inputResource(&_sprite);
        if (ImGui::CollapsingHeader("Preview", ImGuiTreeNodeFlags_DefaultOpen))
            previewSprite(_sprite.asBase());
    }

    void SpriteTool::render(sf::RenderTarget& target)
    {
        if (_sprite && !InputSystem::getActive()->isMouseConsumed())
        {
            sf::Sprite spr(*_sprite->tex, convert(_sprite->rect));
            spr.setOrigin(convert(_sprite->origin.asVector()));
            spr.setPosition(convert(EditorShared::getMouseSnapped().asVector()));
            spr.setColor(sf::Color(255, 255, 255, 128));
            target.draw(spr);
        }
    }
}
