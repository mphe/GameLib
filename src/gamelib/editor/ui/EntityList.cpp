#include "gamelib/editor/ui/EntityList.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "imgui.h"
#include "imgui_internal.h"

namespace gamelib
{
    void drawSearchPopup(bool* open)
    {
        static char buf[256] = { 0 };
        static Entity::Handle ent;

        auto close = [&]() {
            *open = false;
            ent = Entity::Handle();
            memset(buf, 0, sizeof(buf));
        };

        if (ImGui::Begin("Find entity", open, ImGuiWindowFlags_AlwaysAutoResize))
        {
            SelectTool& select = EditorShared::getSelectTool();
            auto entmgr = getSubsystem<EntityManager>();

            if (ImGui::IsWindowAppearing())
                ImGui::SetKeyboardFocusHere();


            if (ImGui::InputText("Search", buf, sizeof(buf)))
            {
                ent = Entity::Handle();
                if (strlen(buf) > 0)
                    for (auto& i : *entmgr)
                        if (i.getName().find(buf) != std::string::npos)
                        {
                            ent = i.getHandle();
                            break;
                        }
            }

            auto entptr = getEntity(ent);

            ImGui::SameLine();

            if (okButton("Select"))
            {
                if (entptr)
                {
                    select.select(entptr);
                    if (entptr && open)
                        close();
                }
                else
                    ImGui::SetKeyboardFocusHere(-1);
            }

            ImGui::SameLine();

            if (open && cancelButton("Cancel"))
                close();

            if (!entptr)
                ImGui::TextColored(sf::Color::Red, "No entity found");
            else
                ImGui::Text("%s", entptr->getName().c_str());
        }
        ImGui::End();
    }

    void drawEntityList(bool* open)
    {
        SelectTool& select = EditorShared::getSelectTool();
        Entity::Handle current;
        bool selectfirst = false;

        if (select.getSelected())
            current = select.getSelected()->getHandle();

        if (ImGui::Begin("Entities", open, ImVec2(250, 285)))
        {
            static char buf[256] = { 0 };

            if (okButton("Select"))
                selectfirst = true;
            ImGui::SameLine();
            if (cancelButton("Clear"))
            {
                memset(buf, 0, sizeof(buf));
                ImGui::SetKeyboardFocusHere(-1);
            }

            ImGui::SameLine();
            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
            ImGui::InputText("Filter", buf, sizeof(buf));
            ImGui::PopItemWidth();

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

                        if (selectfirst)
                        {
                            selectfirst = false;
                            current = handle;
                            select.select(handle);
                        }

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
