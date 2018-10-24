#include "gamelib/editor/ui/JsonView.hpp"
#include "gamelib/core/rendering/Scene.hpp"
#include "gamelib/core/res/ResourceManager.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/editor/EditorShared.hpp"
#include "gamelib/editor/tools/SelectTool.hpp"
#include "gamelib/editor/ui/FileDialog.hpp"
#include "imgui.h"
#include "json/json.h"

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
        out->clear();
        stringout->clear();

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

        *stringout = out->toStyledString();
    }

    void drawJsonView(bool* isopen)
    {
        static Json::Value node;
        static std::string jsonstring;
        static int selected = 0;
        static FileDialog savedlg(FileDialog::Save);

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
                        savedlg.open();
                    ImGui::EndChild();
                }

                if (savedlg.process())
                    writeJsonToFile(savedlg.getPath(), node);

                ImGui::EndGroup();
            }
        }
        ImGui::End();
    }


    void drawEntityFactory(bool* isopen)
    {
        static Json::Value node, diff, norm;
        static std::string jsonstring, diffstring, normstring;
        static FileDialog savedlg(FileDialog::Save);
        static std::string current;
        static bool refresh = false;
        static bool diffsave = false;

        auto selected = EditorShared::getSelectTool().getSelected();
        auto factory = getSubsystem<EntityFactory>();

        if (!factory)
            return;

        if (ImGui::Begin("Entity Factory", isopen, ImVec2(300, 300)))
        {
            { // left
                ImGui::BeginChild("left pane", ImVec2(150, 0), true);
                factory->iterkeys([&](const std::string& name) {
                        if (ImGui::Selectable(name.c_str(), name == current) || current.empty() || refresh)
                        {
                            if (refresh && name != current)
                                return false;

                            refresh = false;
                            current = name;
                            diff.clear();
                            norm.clear();
                            node = *factory->findEntity(name);
                            jsonstring = node.toStyledString();
                            normalizeConfig(node, &norm, *factory);
                            normstring = norm.toStyledString();

                            if (selected && selected->getName() == name)
                            {
                                Json::Value entcfg;
                                writeToJson(entcfg, *selected);
                                diffJson(entcfg, norm, &diff);
                                diff["name"] = selected->getName();
                                diffstring = diff.toStyledString();
                            }
                            else
                                diffstring.clear();
                        }
                        return false;
                    });
                ImGui::EndChild();
            }

            ImGui::SameLine();

            { // right
                ImGui::BeginGroup();

                ImGui::BeginChild("json view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()));
                if (ImGui::CollapsingHeader("Template", ImGuiTreeNodeFlags_DefaultOpen))
                    ImGui::TextUnformatted(jsonstring.c_str(), jsonstring.c_str() + jsonstring.size());
                if (ImGui::CollapsingHeader("Normalized"))
                    ImGui::TextUnformatted(normstring.c_str(), normstring.c_str() + normstring.size());
                if (ImGui::CollapsingHeader("Diff"))
                    ImGui::TextUnformatted(diffstring.c_str(), diffstring.c_str() + diffstring.size());
                ImGui::EndChild();

                ImGui::BeginChild("buttons");
                if (ImGui::Button("Refresh"))
                    refresh = true;

                ImGui::SameLine();
                if (ImGui::Button("Save as"))
                    savedlg.open();

                ImGui::SameLine();
                if (!diffstring.empty() && ImGui::Button("Save diff as"))
                {
                    savedlg.open();
                    diffsave = true;
                }

                ImGui::EndChild();

                if (savedlg.process())
                {
                    if (diffsave)
                        writeJsonToFile(savedlg.getPath(), diff);
                    else
                        writeJsonToFile(savedlg.getPath(), node);
                    diffsave = false;
                }

                ImGui::EndGroup();
            }
        }
        ImGui::End();
    }
}
