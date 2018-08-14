#include "gamelib/editor/tools/EntityTool.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/editor/tools/ToolUtils.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "imgui.h"

namespace gamelib
{
    bool _itemGetter(void* entities, int index, const char** out)
    {
        if (index == -1)
            return false;
        *out = static_cast<std::vector<std::string>*>(entities)->at(index).c_str();
        return true;
    }


    EntityTool::EntityTool() :
        _selected(-1)
    {
        refresh();
    }

    void EntityTool::onMousePressed()
    {
        if (_selected >= 0)
        {
            auto& pos = EditorShared::getMouseSnapped();
            auto handle = createEntity(_entities[_selected], pos.x, pos.y);
            EditorShared::getSelectTool().select(handle);
        }
    }

    void EntityTool::refresh()
    {
        std::string current;
        if (_selected >= 0)
            current = std::move(_entities[_selected]);

        _selected = 0;
        _entities.clear();

        auto factory = EntityFactory::getActive();
        if (factory)
        {
            _entities.reserve(factory->size());

            factory->iterkeys([&](const std::string& name) {
                    if (name.find("brush_") != std::string::npos)
                        return false;
                    if (name == current)
                        _selected = _entities.size();
                    _entities.push_back(name);
                    return false;
                });
        }

        if (_entities.empty())
            _selected = -1;

        std::sort(_entities.begin(), _entities.end());
    }

    void EntityTool::drawGui()
    {
        ImGui::Combo("Entity", &_selected, _itemGetter, (void*)&_entities, _entities.size());
    }

    void EntityTool::render(sf::RenderTarget& target)
    {
        if (!ImGui::GetIO().WantCaptureMouse)
            drawDragBox(target, EditorShared::getMouseSnapped());
    }
}
