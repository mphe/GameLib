#include "editor/editor/ui/JsonView.hpp"
#include "imgui.h"
#include "json/json.h"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "editor/editor/EditorShared.hpp"
#include "editor/editor/tools/SelectTool.hpp"
#include <string>

namespace gamelib
{
    static constexpr const char* items[] = {
        "Current Entity",
        "Scene",
        "ResourceManager",
    };

    void refresh(int selected, std::string* out)
    {
        JsonSerializer* obj = nullptr;

        switch (selected)
        {
            default:
            case 0:
                obj = EditorShared::getSelectTool().getSelected();
                break;

            case 1:
                obj = Scene::getActive();
                break;

            case 2:
                obj = ResourceManager::getActive();
                break;
        }

        if (obj)
        {
            Json::Value json;
            obj->writeToJson(json);
            *out = json.toStyledString();
        }
        else
            out->clear();
    }

    void drawJsonView(bool* isopen)
    {
        static std::string jsonstring;
        static int selected = 0;

        if (!isopen || *isopen)
        {
            ImGui::Begin("Json Viewer", isopen, ImVec2(300, 300));

            { // left
                ImGui::BeginChild("left pane", ImVec2(150, 0), true);
                for (int i = 0; i < 3; ++i)
                    if (ImGui::Selectable(items[i], selected == i))
                    {
                        selected = i;
                        refresh(selected, &jsonstring);
                    }
                ImGui::EndChild();
            }

            ImGui::SameLine();

            { // right
                ImGui::BeginGroup();

                { // json
                    ImGui::BeginChild("json view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
                    ImGui::Text("%s", jsonstring.c_str());
                    ImGui::EndChild();
                }
                { // butons
                    ImGui::BeginChild("buttons");
                    if (ImGui::Button("Refresh"))
                        refresh(selected, &jsonstring);

                    ImGui::SameLine();
                    if (ImGui::Button("Save as"))
                    {
                        // TODO
                    }
                    ImGui::EndChild();
                }

                ImGui::EndGroup();
            }

            ImGui::End();
        }
    }
}
