#ifndef GAMELIB_UI_INPUTS_HPP
#define GAMELIB_UI_INPUTS_HPP

#include <string>
#include "imgui.h"

namespace gamelib
{
    class RenderComponent;
    class SceneData;
    class Transformable;
    class PropertyHandle;
    class Entity;

    bool okButton(const char* label = "OK", const ImVec2& size = ImVec2(0, 0));
    bool cancelButton(const char* label = "Cancel", const ImVec2& size = ImVec2(0, 0));
    bool defaultButton(const char* label, int key, const ImVec2& size = ImVec2(0, 0));

    bool inputBitflags(unsigned int* flags, int num, const char* const* names);

    void inputRenderComponent(RenderComponent& ren);
    void inputSceneData(SceneData& sd);

    bool inputTransform(Transformable& trans);
    bool inputTransform(Transformable& trans, unsigned int transflags);

    bool inputProperty(const std::string& name, const PropertyHandle& handle);
    void inputEntity(Entity& ent);
    void inputEntityProps(Entity& ent);
}

#endif
