#include "gamelib/editor/EntityDesigner.hpp"
#include "gamelib/core/ecs/EntityManager.hpp"
#include "gamelib/core/ecs/EntityFactory.hpp"
#include "gamelib/core/ecs/serialization.hpp"
#include "gamelib/imgui/inputs.hpp"
#include "imgui.h"

namespace gamelib
{
    EntityDesigner::EntityDesigner() :
        _open(false)
    { }

    EntityDesigner::~EntityDesigner()
    {
        close();
    }

    void EntityDesigner::open()
    {
        if (!_open)
        {
            _open = true;
            _ent = EntityManager::getActive()->add("New Entity");
        }
    }

    void EntityDesigner::close()
    {
        _open = false;
        if (!_ent.isNull())
        {
            getEntity(_ent)->destroy();
            _ent = Entity::Handle();
        }
    }

    void EntityDesigner::draw()
    {
        if (!_open)
            return;

        bool chooseload = false;
        bool choosesave = false;
        const char* chosenPath;
        Entity& ent = *getEntity(_ent);

        ImGui::Begin("Entity Designer", &_open, ImGuiWindowFlags_MenuBar);
        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New"))
                    ent.clear();

                if (ImGui::MenuItem("Save"))
                {
                    if (currentFilePath.empty())
                        choosesave = true;
                    else
                        writeToFile(currentFilePath, ent);
                }

                if (ImGui::MenuItem("Save as"))
                    choosesave = true;

                if (ImGui::MenuItem("Load"))
                    chooseload = true;

                if (ImGui::MenuItem("Close"))
                    _open = false;

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
                ImGui::EndMenu();
            if (ImGui::BeginMenu("View"))
                ImGui::EndMenu();
            ImGui::EndMenuBar();
        }

        { // Saving / Loading
            chosenPath = loaddlg.chooseFileDialog(chooseload);
            if (strlen(chosenPath) > 0)
            {
                currentFilePath = chosenPath;
                Json::Value file;
                if (loadJsonFromFile(currentFilePath, file))
                    EntityFactory::getActive()->createFromJson(file, &ent);
            }

            chosenPath = savedlg.saveFileDialog(choosesave);
            if (strlen(chosenPath) > 0)
            {
                currentFilePath = chosenPath;
                writeToFile(currentFilePath, ent);
            }
        }


        inputEntityProps(ent);

        ImGui::PopItemWidth();
        ImGui::End();

        // if closed
        if (!_open)
            close();
    }
}
