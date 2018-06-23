#include "editor/editor/ui/EntityList.hpp"
#include "editor/editor/EditorShared.hpp"
#include "editor/editor/tools/SelectTool.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "imgui.h"

namespace gamelib
{
    void drawEntityList(bool* open)
    {
        SelectTool& select = EditorShared::getSelectTool();
        Entity::Handle current;

        if (select.getSelected())
            current = select.getSelected()->getHandle();

        if (ImGui::Begin("Entities", open, ImVec2(250, 285)))
        {
            static char buf[256] = { 0 };

            ImGui::InputText("Filter", buf, sizeof(buf));
            ImGui::SameLine();
            if (ImGui::Button("Clear"))
                memset(buf, 0, sizeof(buf));

            if (ImGui::ListBoxHeader("##entities", ImVec2(-1, -1)))
            {
                auto entmgr = getSubsystem<EntityManager>();
                if (entmgr)
                {
                    for (auto& i : *entmgr)
                    {
                        if (strlen(buf) > 0 && i.getName().find(buf) == std::string::npos)
                            continue;

                        auto handle = i.getHandle();
                        ImGui::PushID(handle.index);
                        if (ImGui::Selectable(i.getName().c_str(), current == handle))
                        {
                            current = handle;
                            select.select(handle);
                        }
                        ImGui::PopID();
                    }
                }

                ImGui::ListBoxFooter();
            }
        }
        ImGui::End();
    }
}
