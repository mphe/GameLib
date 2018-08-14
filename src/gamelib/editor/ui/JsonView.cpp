#include "gamelib/editor/ui/JsonView.hpp"
#include "imgui.h"
#include "imguifilesystem.h"
#include "json/json.h"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include <string>

namespace gamelib
{
    static constexpr const char* items[] = {
        "Current Entity",
        "Scene",
        "ResourceManager",
    };

    void refresh(int selected, Json::Value* out, std::string* stringout)
    {
        JsonSerializer* obj = nullptr;
        Entity* ent = nullptr;

        switch (selected)
        {
            default:
            case 0:
                ent = EditorShared::getSelectTool().getSelected();
                break;

            case 1:
                obj = Scene::getActive();
                break;

            case 2:
                obj = ResourceManager::getActive();
                break;
        }

        if (obj || ent)
        {
            if (ent)
                writeToJson(*out, *ent);
            else
                obj->writeToJson(*out);
        }
        else
            out->clear();

        *stringout = out->toStyledString();
    }

    void drawJsonView(bool* isopen)
    {
        static Json::Value node;
        static std::string jsonstring;
        static int selected = 0;
        static ImGuiFs::Dialog savedlg;
        bool choosesave = false;

        if (ImGui::Begin("Json Viewer", isopen, ImVec2(300, 300)))
        {
            { // left
                ImGui::BeginChild("left pane", ImVec2(150, 0), true);
                for (int i = 0; i < 3; ++i)
                    if (ImGui::Selectable(items[i], selected == i))
                    {
                        selected = i;
                        refresh(selected, &node, &jsonstring);
                    }
                ImGui::EndChild();
            }

            ImGui::SameLine();

            { // right
                ImGui::BeginGroup();

                { // json
                    ImGui::BeginChild("json view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
                    ImGui::TextUnformatted(jsonstring.c_str(), jsonstring.c_str() + jsonstring.size());
                    ImGui::EndChild();
                }
                { // buttons
                    ImGui::BeginChild("buttons");
                    if (ImGui::Button("Refresh"))
                        refresh(selected, &node, &jsonstring);

                    ImGui::SameLine();
                    if (ImGui::Button("Save as"))
                        choosesave = true;
                    ImGui::EndChild();
                }

                if (strlen(savedlg.saveFileDialog(choosesave, savedlg.getLastDirectory())) > 0)
                    writeJsonToFile(savedlg.getChosenPath(), node);

                ImGui::EndGroup();
            }
        }
        ImGui::End();
    }
}
