#include "gamelib/editor/ui/EntityList.hpp"
#include "gamelib/imgui/buttons.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
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
        auto entmgr = getSubsystem<EntityManager>();
        if (!entmgr)
            return;

        Entity::Handle current;
        SelectTool& select = EditorShared::getSelectTool();

        if (select.getSelected())
            current = select.getSelected()->getHandle();

        if (ImGui::Begin("NewEntities", open, ImVec2(250, 285)))
        {
            static ImGuiTextFilter filter;
            filter.Draw();

            for (auto& i : *entmgr)
            {
                auto entname = i.getName().c_str();
                auto handle = i.getHandle();

                if (!filter.PassFilter(entname))
                    continue;

                ImGui::PushID(handle.index);
                auto flags = current == handle ? ImGuiTreeNodeFlags_Selected : 0;
                bool open = ImGui::TreeNodeEx(entname, flags);

                if (ImGui::IsItemClicked())
                    select.select(handle);

                if (open)
                {
                    for (auto& comp : i)
                    {
                        ImGui::PushID(comp.ptr.get());
                        auto name = generateName(comp.ptr->getName(), comp.id);
                        flags = select.getSelectedComponent() == comp.ptr.get()? ImGuiTreeNodeFlags_Selected : 0;
                        ImGui::TreeNodeEx(name.c_str(), flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
                        if (ImGui::IsItemClicked())
                            select.selectComponent(comp.ptr.get());
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
                ImGui::PopID();
            }
        }
        ImGui::End();
    }

    void drawEntityProperties(bool* open)
    {
        SelectTool& select = EditorShared::getSelectTool();
        auto ent = select.getSelected();

        if (!ent)
            return;

        auto comp = select.getSelectedComponent();

        if (ImGui::Begin("NewProperties", open, ImVec2(250, 285)))
        {
            if (comp)
                inputComponent(*comp);
            else
                inputEntityProps(*ent);
        }
        ImGui::End();
    }
}
