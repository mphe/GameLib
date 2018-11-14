#ifndef GAMELIB_IMGUI_INPUTS_HPP
#define GAMELIB_IMGUI_INPUTS_HPP

#include <string>
#include "imgui.h"
#include "gamelib/core/rendering/Layer.hpp"

namespace gamelib
{
    class RenderComponent;
    class SceneData;
    class Transformable;
    class PropertyHandle;
    class PropertyContainer;
    class Entity;
    class Component;

    bool okButton(const char* label = "OK", const ImVec2& size = ImVec2(0, 0));
    bool cancelButton(const char* label = "Cancel", const ImVec2& size = ImVec2(0, 0));
    bool defaultButton(const char* label, int key, const ImVec2& size = ImVec2(0, 0));

    bool inputBitflags(const char* label, unsigned int* flags, int num, const char* const* names);

    bool inputLayer(const char* label, Layer::Handle* handle);

    void inputRenderComponent(RenderComponent& ren);
    void inputSceneData(SceneData& sd);

    bool inputTransform(Transformable& trans);

    bool inputProperties(const PropertyContainer& props);
    bool inputProperty(const std::string& name, const PropertyHandle& handle);
    void inputEntityProps(Entity& ent);
    void inputComponent(Component& comp);
}

#endif
